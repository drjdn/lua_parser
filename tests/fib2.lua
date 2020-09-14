-- fibonacci function with cache

a=[==[this is a
 
    "string"  ]==]

--[==[ long comments [are] fun ]==]

--[======[  --[[  Bite me evil comment!  
]======]

--[=[  blah blah [======[ ]====] 
]=]

-- very inefficient fibonacci function
function fib(n)
	N=N+1
	if n<2 then
		return n
	else
		return fib(n-1)+fib(n-2)
	end
end

-- a general-purpose value cache
function cache(f)
	local c={}
	return function (x)
		local y=c[x]
		if not y then
			y=f(x)
			c[x]=y
		end
		return y
	end
end

-- run and time it
function test(s,f)
	N=0
	local c=os.clock()
	local v=f(n)
	local t=os.clock()-c
	print(s,n,v,t,N)
end

n=arg[1] or 24		-- for other values, do lua fib.lua XX
n=tonumber(n)
print("","n","value","time","evals")
test("plain",fib)
fib=cache(fib)
test("cached",fib)

local yield,wrap = coroutine.yield,coroutine.wrap
local strfind = string.find
local strsub = string.sub
local append = table.insert

local function assert_arg(idx,val,tp)
    if type(val) ~= tp then
        error("argument "..idx.." must be "..tp, 2)
    end
end

local lexer = {}

local NUMBER1 = '^0x[%da-fA-F]+'
local NUMBER2 = '^%d+%.?%d*[eE][%+%-]?%d+'
local NUMBER3 = '^%d+%.?%d*'
local IDENT = '^[%a_][%w_]*'
local WSPACE = '^%s+'
local STRING1 = [[^'.-[^\\]']]
local STRING2 = [[^".-[^\\]"]]
local STRING3 = "^((['\"])%2)" -- empty string

local lua_matches, lua_keyword

local function tdump(tok)
    return yield(tok,tok)
end

local function ndump(tok)
    tok = tonumber(tok)
    return yield("number",tok)
end

-- regular strings, single or double quotes; usually we want them
-- without the quotes
local function sdump(tok)
    tok = tok:sub(2,-2)
    return yield("string",tok)
end

-- long Lua strings need extra work to get rid of the quotes
local function sdump_l(tok)
    tok = tok:sub(3,-3)
    return yield("string",tok)
end

local function cdump(tok)
    return yield('comment',tok)
end

local function wsdump (tok)
    return yield("space",tok)
end

local function plain_vdump(tok)
    return yield("ident",tok)
end

local function lua_vdump(tok)
    if lua_keyword[tok] then
        return yield("keyword",tok)
    else
        return yield("ident",tok)
    end
end

local function lcount()
   local i=0
   local function update()
      i=i+1
      return i
   end
   return update
end

--- create a plain token iterator from a string or file-like object.
-- @param s the string
-- @param matches an optional match table (set of pattern-action pairs)
-- @param filter a table of token types to exclude, by default {space=true}
-- which means convert numbers and strip string quotes.
function lexer.scan (s,matches,filter)
    local file = type(s) ~= 'string' and s
    filter = filter or {space=true}
    if filter then
        if filter.space then filter[wsdump] = true end
        if filter.comments then
            filter[cdump] = true
        end
    end
    local function lex ()
        local i1,i2,idx,res1,res2,tok,pat,fun,capt
        local line = 1
        if file then s = file:read()..'\n' end
        local sz = #s
        local idx = 1
        while true do
            for _,m in ipairs(matches) do
                pat = m[1]
                fun = m[2]
                i1,i2 = strfind(s,pat,idx)
                if i1 then
                    tok = strsub(s,i1,i2)
                    idx = i2 + 1
		    print(filter and filter[fun])
                    if not (filter and filter[fun]) then
                        lexer.finished = idx > sz
                        res1,res2 = fun(tok)
                    end
                    if res1 then
                        local tp = type(res1)
                        -- insert a token list
                        if tp=='table' then
                            yield('','')
                            for _,t in ipairs(res1) do
                                yield(t[1],t[2])
                            end
                        elseif tp == 'string' then -- or search up to some special pattern
                            i1,i2 = strfind(s,res1,idx)
                            if i1 then
                                tok = strsub(s,i1,i2)
                                idx = i2 + 1
                                yield('',tok)
                            else
                                yield('','')
                                idx = sz + 1
                            end
                            --if idx > sz then return end
                        else
                            yield(line,idx)
                        end
                    end
                    if idx > sz then
                        if file then
                            --repeat -- next non-empty line
                                line = line + 1
                                s = file:read()
                                if not s then return end
                            --until not s:match '^%s*$'
                            s = s .. '\n'
                            idx ,sz = 1,#s
                            break
                        else
                            return
                        end
                    else break end
                end
            end
        end
    end
    return wrap(lex)
end

--- get the Lua keywords as a set-like table.
-- So <code>res["and"]</code> etc would be <code>true</code>.
-- @return a table
function lexer.get_keywords ()
   lua_keyword = {
      ["and"] = true, ["break"] = true,  ["do"] = true,
      ["else"] = true, ["elseif"] = true, ["end"] = true,
      ["false"] = true, ["for"] = true, ["function"] = true,
      ["if"] = true, ["in"] = true,  ["local"] = true, ["nil"] = true,
      ["not"] = true, ["or"] = true, ["repeat"] = true,
      ["return"] = true, ["then"] = true, ["true"] = true,
      [ "until"] = true,  ["while"] = true
   }    
   return lua_keyword
end

--- create a Lua token iterator from a string or file-like object.
-- Will return the token type and value.
-- @param s the string
-- @param filter a table of token types to exclude, by default {space=true,comments=true}
-- which means convert numbers and strip string quotes.
function lexer.lua(s,filter)
    filter = filter or {space=true,comments=true}
    lexer.get_keywords()
    lua_matches = {
       {WSPACE,wsdump},
       {NUMBER1,ndump},
       {IDENT,lua_vdump},
       {NUMBER2,ndump},
       {NUMBER3,ndump},
       {STRING3,sdump},
       {STRING1,sdump},
       {STRING2,sdump},
       {'^%-%-%[%[.-%]%]',cdump},
       {'^%-%-.-\n',cdump},
       {'^%[%[.-%]%]',sdump_l},
       {'^==',tdump},
       {'^~=',tdump},
       {'^<=',tdump},
       {'^>=',tdump},
       {'^%.%.%.',tdump},
       {'^%.%.',tdump},
       {'^.',tdump}
    }
    return lexer.scan(s,lua_matches,filter)
end

return lexer

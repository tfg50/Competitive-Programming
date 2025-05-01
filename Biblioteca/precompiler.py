import pyperclip

defines = {}

def go(path):
    f = open(path, "r")
    ans = []
    stack = []
    for line in f:
        if line[-1] == '\n':
            line = line[:-1]
        if line.startswith("#ifndef "):
            to_check = line[8:]
            if to_check in defines:
                stack.append(True)
            else:
                stack.append(False)
        elif line.startswith("#ifdef "):
            to_check = line[8:]
            if to_check in defines:
                stack.append(False)
            else:
                stack.append(True)
        elif line == "#endif":
            stack.pop()
        elif line == "#else":
            stack[-1] = not stack[-1]
        elif True in stack:
            continue
        elif line.startswith("#define "):
            defines[line[8:]] = "valid"
        elif line.startswith("#include \""):
            ans += go(line[10:-1])
        else:
            ans.append(line)
    return ans

raw_text = go("x.cpp")
empty_line = True
text = ""
for line in raw_text:
    if line == "" and empty_line:
        continue
    text += line + "\n"
    # print(line)
    empty_line = line == ""

pyperclip.copy(text)
# print(text)
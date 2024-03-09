csv = open("SDL_Scancodes.csv", "r")
lines = csv.readlines()
csv.close()

scancodes = {}
for line in lines[1:]:
    line = line.strip().replace('"', '').split(",")
    if "(" in line[0] and ")" in line[0]:
        line[0] = line[0][:line[0].find("(") - 1]
    scancodes[line[0]] = line[1]

for key in scancodes:
    print(f'{{"{key}", {scancodes[key]}}},')

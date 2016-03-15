filename = input("File to dedup: ")
f = open(filename, 'r')
entries = {}
writestr = ''
for line in f:
    if not entries.__contains__(line):
        entries[line] = True
        writestr += line
f.close()
f = open(filename, 'w')
f.write(writestr)
f.close()

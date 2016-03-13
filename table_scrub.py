'''If popularity high enough for certain board configuration, leave it, otherwise delete it.'''

filenames = ['tables/transtable.txt', 'tables/openingbook.txt']
pop_threshold = 20

for filename in filenames:
	totalwrite = ''
	f = open(filename, 'r')
	for line in f:
		entry = (line.strip()).split()	
		if int(entry[4]) >= pop_threshold:
			totalwrite += line
	f.close()
	f = open(filename, 'w')
	f.write(totalwrite)
	f.close()




''' Takes in a file containing algebraic notation for Othello games	
    Outputs transposition table format <board> <move> <score> <depth> <pop> ''' 

import copy 
import string

BOARDSIZE = 8
def generateStartBoard():
	startingBoard = ['', '', '', '', '', '', '', '', \
			 '', '', '', '', '', '', '', '', \
			 '', '', '', '', '', '', '', '', \
			 '', '', '', 'b', 'w', '', '', '', \
			 '', '', '', 'w', 'b', '', '', '', \
			 '', '', '', '', '', '', '', '', \
			 '', '', '', '', '', '', '', '', \
			 '', '', '', '', '', '', '', '',]
	return startingBoard

def printBoard(b):
	printStr = ''
	for i in range(BOARDSIZE):
		for j in range(BOARDSIZE):
			if b[index(i,j)]:
				printStr += b[index(i,j)]
			else:
				printStr += ' '
		printStr += '\n'
	print(printStr)
	
def index(x, y):
	#return 8 * x + y
	return x + 8*y
def doMove(board, move, black):
	moveNums = moveToString(move)
	x = int(moveNums[0])
	y = int(moveNums[1])
	i = index(x, y)
	if(black):
		board[i] = 'b'
	else:
		board[i] = 'w'
	return moveNums

def boardToString(b):
	ret = ''
	for i in b:
		if i:
			ret += '1'
		else:
			ret += '0'
	for i in b:
		if i == 'b': 
			ret += '1'
		else:
			ret += '0'
	return ret
def moveToString(move):
	movex = move[0]
	movey = int(move[1]) - 1
	movey = str(movey)
	ret = str(string.ascii_lowercase.index(movex)) + movey
	return ret

def swap(board, i1, i2):
	temp = board[i1]
	board[i1] = board[i2]
	board[i2] = temp

def refBoard(b):
	'''reflects board to capture a symmetrical position -- over the line x = y'''
	for j in range(BOARDSIZE):
		#swap(b, index(BOARDSIZE-1-i,BOARDSIZE-1-i), index(i,i))
		for i in range(BOARDSIZE-j):
			swap(b, index(BOARDSIZE-1-j, BOARDSIZE-1-i), index(i, j))
def refMove(m):
	movex = int(m[0])
	movey = int(m[1])
	newx = BOARDSIZE - 1 - movey
	newy = BOARDSIZE - 1 - movex
	return str(newx)+str(newy)

def rotBoard(b):
	'''rotates board to capture a symmetrical position -- 180 deg'''
	
	for i in range(int(BOARDSIZE/2)):
		for j in range(BOARDSIZE):
			swap(b, index(i, j), index(BOARDSIZE-1-i, BOARDSIZE-1-j))
def rotMove(m):
	movex = int(m[0])
	movey = int(m[1]) 
	newx = BOARDSIZE - 1 - movex
	newy = BOARDSIZE - 1 - movey
	return str(newx) + str(newy)

print('Board b for testing ... ')
b = generateStartBoard()
for i in '12345678':
  doMove(b, 'a' + i, True)
for i in 'abcdefgh':
  doMove(b, i + '1', False)
printBoard(b)

if __name__ == '__main__':
	readfile = input("Enter the filename of the openings: ")
	f = open(readfile, 'r')
	writefile = input("Enter the file to write to: ")
	startingBoard = generateStartBoard()
	opening_book_string = ''
	movecount = 0
	for line in f:
		black = True
		curr_board = copy.deepcopy(startingBoard)
		movenum = 0
		for str_ind in range(0, int(len(line)), 2):
			if movenum >= 15:
				break
		        #Regular Move
			movestring = line[str_ind] + line[str_ind + 1]
			move = doMove(curr_board, movestring, black)
			boardKey = boardToString(curr_board)
			opening_book_string += boardKey + ' 0 ' + move + ' 0 0\n'
		        #Rotated Move
			rotB = copy.deepcopy(curr_board)
			rotBoard(rotB)
			rotM = move
			rotM = rotMove(rotM)
			rotBoardKey = boardToString(rotB)
			opening_book_string += rotBoardKey + ' 0 ' + rotM + ' 0 0\n'
			#Reflected Move
			refB = copy.deepcopy(curr_board)
			refBoard(refB)
			refM = move
			refM = refMove(refM)
			refBoardKey = boardToString(refB)
			opening_book_string += refBoardKey + ' 0 ' + refM + ' 0 0\n'
		        #Ref Rot move
			rrB = copy.deepcopy(curr_board)
			refBoard(rrB)
			rotBoard(rrB)
			rrM = move
			rrM = refMove(rrM)
			rrM = rotMove(rrM)
			rrBoardKey = boardToString(rrB)
			opening_book_string += rrBoardKey + ' 0 ' + rrM + ' 0 0\n'
			#Switch Turns
			black = not black
			movenum += 1
			movecount += 4
	f.close()
	f = open(writefile, 'w')
	f.write(opening_book_string)
	f.close() 
	print(movecount)
		
		
		




''' Takes in a file containing algebraic notation for Othello games	
    Outputs transposition table format <board> <move> <score> <depth> <pop> ''' 

import copy 
import string

BOARDSIZE = 8
def index(x, y):
	return x + 8*y
def doMove(board, move, black):
	moveNums = moveToString(move)
	x = moveNums[0]
	y = moveNums[1]
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
	movey = move[1]
	ret = str(string.lowercase.index(movex)) + movey
	return ret

def swap(board, i1, i2):
	temp = board[i1]
	board[i1] = board[i2]
	board[i2] = temp

def refBoard(b):
	'''reflects board to capture a symmetrical position -- over the line x = y'''
	for i in range(BOARDSIZE):
		swap(b, index(BOARDSIZE-1-i,BOARDSIZE-1-i), index(i,i))
		for j in range(BOARDSIZE-i):
			swap(b, index(BOARDSIZE-1-i-j, BOARDSIZE-1-i), index(i, i+j))
			swap(b, index(BOARDSIZE-1-i, BOARDSIZE-1-i-j), index(i+j, i))
def refMove(m):
	movex = int(m[0])
	movey = int(m[1])
	raise

def rotBoard(b):
	'''rotates board to capture a symmetrical position -- 180 deg'''
	for i in range(BOARDSIZE/2):
		for j in range(BOARDSIZE):
			swap(b, index(i, j), index(BOARDSIZE-1-i, BOARDSIZE-1-j))
def rotMove(m):
	movex = int(m[0])
	movey = int(m[1]) 
	raise

readfile = input("Enter the filename of the openings: ")
f = open(readfile, 'r')
writefile = input("Enter the file to write to: ")
startingBoard = ['', '', '', '', '', '', '', '', \
		'', '', '', '', '', '', '', '', \
		'', '', '', '', '', '', '', '', \
		'', '', '', 'b', 'w', '', '', '', \
		'', '', '', 'w', 'b', '', '', '', \
		'', '', '', '', '', '', '', '', \
		'', '', '', '', '', '', '', '', \
		'', '', '', '', '', '', '', '',]

opening_book_string = ''
for line in f:
	black = True
	curr_board = copy.deepcopy(startingBoard)
	for str_ind in range(len(line)/2, 12):
		#Regular Move
		movestring = line[str_ind] + line[str_ind]
		move = doMove(curr_board, movestring, black)
		boardKey = boardToString(curr_board)
		opening_book_string += boardKey + '0 ' + move + ' 0 0\n'
		#Rotated Move
		rotB = copy.deepcopy(curr_board)
		rotBoard(rotB)
		rotM = move
		rotM = rotMove(rotM)
		rotBoardKey = boardToString(rotB)
		opening_book_string += rotBoardKey + '0 ' + rotM + ' 0 0\n'
		#Reflected Move
		refB = copy.deepcopy(curr_board)
		refBoard(refB)
		refM = move
		refM = refMove(refM)
		refBoardKey = boardToString(refB)
		opening_book_string += refBoardkey + '0 ' + refM + ' 0 0\n'
		#Ref Rot move
		rrB = copy.deepcopy(curr_board)
		refBoard(rrB)
		rotBoard(rrB)
		rrM = move
		rrM = refMove(rrM)
		rrM = rotMove(rrM)
		rrBoardKey = boardToString(rrB)
		opening_book_string += rrBoardKey + '0 ' + rrM + ' 0 0\n'
f.close()
f.open(writefile, 'w')
f.write(opening_book_string)
f.close() 
		
		
		




 This project is a command-line Chess game written in C. It follows a modular design where separate
 files handle different aspects of the game.


Features:

Board:
  a b c d e f g h
1 r n b q k b n r
2 p p p p p p p p
3 . . . . . . . .
4 . . . . . . . .
5 . . . . . . . .
6 . . . . . . . .
7 P P P P P P P P
8 R N B Q K B N R
  a b c d e f g h


main Loop:
    check whose move
    input 
    check is move valid
    do move
    is enemy in check
    check if game over
    turn switch

check whose move:
    if move % 2 == 0 black
    else white

check if move is valid:
    is the piece of the right color
    no same color piece in the destination
    move is valid for the piece
    is not under check
OR
    check if move is in all valid moves for color list

game:
    make a list all valid piece moves for both color
    is under check:
        check if any of all opposing color valid piece moves hits the king
    is checkmated:
        if there's no valid move for the opposing color and opponent is under check
    is stalemate:
        if no valid move is available for opponent and opponent is not under check
        if Only 1 Bishop or 1 Knight exist
        if nothing exists but kings
        if threefold repetition
        

import random,time
import pygame
from pygame.locals import *


X_IMAGE=pygame.image.load("x.png")
O_IMAGE=pygame.image.load("o.png")


class GameBoard(object):
    def __init__(self):
        self.board = [None,None,None,
                      None,None,None,
                      None,None,None,]
        self.winner = None
        
    def draw(self,screen):
        screen.fill((0,0,0))
        pygame.draw.aaline(screen, (255,255,255), (0,120), (360,120), 1)
        pygame.draw.aaline(screen, (255,255,255), (0,240), (360,240), 1)
        pygame.draw.aaline(screen, (255,255,255), (120,0), (120,360), 1)
        pygame.draw.aaline(screen, (255,255,255), (240,0), (240,360), 1)
        num=0
        for i in self.board:
            num+=1
            x=(((num%3)+2)%3)*120
            if num<4:
                y=0
            elif num<7:
                y=120
            else:
                y=240
                
            if i=="O":
                self.draw_o(screen,(x,y))
            elif i=="X":
                self.draw_x(screen,(x,y))
        pygame.display.flip()

    def draw_o(self,screen,pos):
        screen.blit(O_IMAGE,pos)

    def draw_x(self,screen,pos):
        screen.blit(X_IMAGE,pos)
        
    def check(self):
        lines=[self.board[:3],self.board[3:6],self.board[6:],
              [self.board[0],self.board[3],self.board[6]],
              [self.board[1],self.board[4],self.board[7]],
              [self.board[2],self.board[5],self.board[8]],
              [self.board[0],self.board[4],self.board[8]],
              [self.board[2],self.board[4],self.board[6]]]
        for i in lines:
            if i == ["O","O","O"]:
                self.winner="O"
            elif i == ["X","X","X"]:
                self.winner="X"
        test=True
        for space in self.board:
            if space==None:
                test=False
        if test and self.winner==None:
            self.winner="draw"


class Player(object):
    def __init__(self,player):
        self.player=player
        self.board=[None,None,None,
            None,None,None,
            None,None,None,]
        self.moved=False
        if player=="O":
            self.opponent="X"
        else:
            self.opponent="O"

    def __str__(self):
        return "Player"

    def move(self,board):
        self.moved=False
        while not self.moved:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    done=True
                    break;
                elif event.type == pygame.KEYDOWN:
                    if event.key == K_EXCAPE:
                        done=True
                        break;
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    if event.button == 1:
                        x,y=event.pos
                        pos=0
                        if y<120:
                            pos+=0
                        elif y<240:
                            pos+=3
                        else:
                            pos+=6
                        if x<120:
                            pos+=0
                        elif x<240:
                            pos+=1
                        else:
                            pos+=2
                        if board[pos]==None:
                            board[pos]=self.player
                            self.moved=True
        return board
        

class Computer(object):
    def __init__(self,player):
        self.player=player
        self.board=[None,None,None,
            None,None,None,
            None,None,None,]
        self.moved=False
        if player=="O":
            self.opponent="X"
        else:
            self.opponent="O"

    def __str__(self):
        return "Computer"

    def move(self,board):
        time.sleep(0.2)
        self.board=board
        self.moved=False
        
        #Check to win
        for i in range(9):
            temp=self.board[:]
            if temp[i]==None:
                temp[i]=self.player
                lines=[temp[:3],temp[3:6],temp[6:],
                    [temp[0],temp[3],temp[6]],
                    [temp[1],temp[4],temp[7]],
                    [temp[2],temp[5],temp[8]],
                    [temp[0],temp[4],temp[8]],
                    [temp[2],temp[4],temp[6]]]
                for j in lines:
                    if j == [self.player,self.player,self.player]:
                        self.board[i]=self.player
                        self.moved=True
                        break;
            if self.moved:
                break;
            
        #Checks if opponent can win
        if not self.moved:
            for i in range(9):
                temp=self.board[:]
                if temp[i]==None:
                    temp[i]=self.opponent
                    lines=[temp[:3],temp[3:6],temp[6:],
                        [temp[0],temp[3],temp[6]],
                        [temp[1],temp[4],temp[7]],
                        [temp[2],temp[5],temp[8]],
                        [temp[0],temp[4],temp[8]],
                        [temp[2],temp[4],temp[6]]]
                    for j in lines:
                        if j == [self.opponent,self.opponent,self.opponent]:
                            self.board[i]=self.player
                            self.moved=True
                            break;
                    if self.moved:
                        break;

        #First move = random corner
        if not self.moved:
            blank= [None,None,None,
                      None,None,None,
                      None,None,None,]
            if self.board==blank:
                options=[0,2,6,8]
                self.board[options[random.randint(0,3)]]=self.player
                self.moved=True

        #Stop easy win
        if not self.moved:
            is_first=True
            for i in self.board:
                if i==self.player:
                    is_first=False
            if is_first:
                for i in [0,2,6,8]:
                    if self.board[i]==self.opponent:
                        self.board[8-i]=self.player
                        self.moved=True
                        break;

        #Gain easy win
        if not self.moved:
            if self.board[4]==self.opponent:
                has_corner=False
                only_1=True
                corner=0
                for i in [0,2,6,8]:
                    if self.board[i]==self.player:
                        if has_corner:
                            only_1=False
                        else:
                            has_corner=True
                            corner=i
                if has_corner and only_1:
                    if self.board[8-corner]==None:
                        self.board[8-corner]=self.player
                        self.moved=True

        #Stop players best move
        if not self.moved:
            best_move=None
            best_count=0
            for i in range(9):
                temp=self.board[:]
                if temp[i]==None:
                    temp[i]="test"
                    lines=[temp[:3],temp[3:6],temp[6:],
                        [temp[0],temp[3],temp[6]],
                        [temp[1],temp[4],temp[7]],
                        [temp[2],temp[5],temp[8]],
                        [temp[0],temp[4],temp[8]],
                        [temp[2],temp[4],temp[6]]]
                    count=0
                    for j in lines:
                        is_test=False
                        is_opponent=False
                        not_self=True
                        for k in j:
                            if k=="test":
                                is_test=True
                            elif k==self.opponent:
                                is_opponent=True
                            elif k==self.player:
                                not_self=False
                        if is_test and is_opponent and not_self:
                            count+=1
                    if count>best_count:
                        best_count=count
                        best_move=i
            if best_count>1:
                self.board[best_move]=self.player
                self.moved=True
        
        #Best remaining move           
        if not self.moved:
            best_move=None
            best_count=0
            for i in range(9):
                temp=self.board[:]
                if temp[i]==None:
                    temp[i]="test"
                    lines=[temp[:3],temp[3:6],temp[6:],
                        [temp[0],temp[3],temp[6]],
                        [temp[1],temp[4],temp[7]],
                        [temp[2],temp[5],temp[8]],
                        [temp[0],temp[4],temp[8]],
                        [temp[2],temp[4],temp[6]]]
                    count=0
                    for j in lines:
                        is_test=False
                        is_self=False
                        not_opponent=True
                        for k in j:
                            if k=="test":
                                is_test=True
                            elif k==self.player:
                                is_self=True
                            elif k==self.opponent:
                                not_opponent=False
                        if is_test and is_self and not_opponent:
                            count+=1
                    if count>best_count:
                        best_count=count
                        best_move=i
                    elif count==best_count:
                        if i==0 or i==2 or i==6 or i==8:
                            if not best_move==0 and not best_move==2 and not best_move==6 and not best_move==8:
                                best_move=i
                            else:
                                corner_list=[6,0,2,8,6,0]
                                if i==0:
                                    num=1
                                elif i==2:
                                    num=2
                                elif i==6:
                                    num=3
                                elif i==8:
                                    num=4
                                if self.board[corner_list[num+1]]==self.player and self.board[corner_list[num-1]]==self.player:
                                    choice=random.randint(0,1)
                                    choice_list=[best_move,i]
                                    best_move=choice_list[choice]
                        elif i==4 and (best_move==1 or best_move==3 or best_move==5 or best_move==7 or best_move==None):
                            best_move=i
                        elif best_move==None:
                            best_move=i
            self.board[best_move]=self.player
            self.moved=True
                        
        return self.board
                
        
def setup_game(gameboard,screen):
    chosen=False
    while not chosen:
        player_o=input("Should Player O be:\n1:Human\n2:Computer\n")
        if player_o=="1":
            player_o=Player("O")
            chosen=True
        elif player_o=="2":
            player_o=Computer("O")
            chosen=True
        else:
            print("That is not a valid answer.")
    chosen=False
    while not chosen:
        player_x=input("Should Player X be:\n1:Human\n2:Computer\n")
        if player_x=="1":
            player_x=Player("X")
            chosen=True
        elif player_x=="2":
            player_x=Computer("X")
            chosen=True
        else:
            print("That is not a valid answer.")
    gameboard.board = [None,None,None,
                      None,None,None,
                      None,None,None,]
    gameboard.winner = None
    turn="O"
    restart = False
    for event in pygame.event.get(pygame.VIDEOEXPOSE):
        pass
    while not restart:
        for event in pygame.event.get(pygame.VIDEOEXPOSE):
            if event.type == pygame.VIDEOEXPOSE:
                restart = True
    time.sleep(0.4)
    return player_o,player_x,turn
    
            
def main():
    pygame.init()
    done=False
    gameboard=GameBoard()
    screen = pygame.display.set_mode((360,360))
    pygame.display.set_caption("Tic Tac Toe")
    pygame.display.iconify()
    player_o,player_x,turn=setup_game(gameboard,screen)
    gameboard.draw(screen)
    while not done:
        gameboard.draw(screen)
        if turn=="O":
            gameboard.board=player_o.move(gameboard.board)
            turn="X"
        elif turn=="X":
            gameboard.board=player_x.move(gameboard.board)
            turn="O"
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                done=True
                break;
            elif event.type == pygame.KEYDOWN:
                if event.key == K_EXCAPE:
                    done=True
                    break;
        gameboard.check()
        if gameboard.winner!=None:
            gameboard.draw(screen)
            pygame.display.iconify()
            if gameboard.winner=="draw":
                print("The game was a draw.")
            else:
                print("Player",gameboard.winner,"has won!")
            chosen=False
            while True:
                choice=input("Do you want to play again? Y/N ")
                if choice=="y" or choice=="Y":
                    player_o,player_x,turn=setup_game(gameboard,screen)
                    break;
                elif choice=="n" or choice=="N":
                    done=True
                    break;
                else:
                    print("That is an invalid answer!")


main()
pygame.quit()

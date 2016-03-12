/**
 * \file      Catch_me.cpp
 * \author    Mathis Garcia, Arnaud Sanchez, Jérémy Topalian, Loïc Vigne.
 * \version   2.0
 * \date      17 Decembre 2013
 * \brief     Jeu "Catch me if you can".
 *
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <utility>
#include <cstdlib>
#include <fstream>

using namespace std;

/** \namespace catch_me
 * 
 * namespace grouping tools components
 * game's gameplay
 */

namespace catch_me
{
	typedef vector <char> CVLine; /**  \typedef Vector \brief a type representing a row of the matrix */
	typedef vector <CVLine> CMatrix; /**  \typedef \brief Vector: a type representing the matrix  */
	typedef pair <unsigned, unsigned> CPosition; /**  \typedef \brief pair: a type representing a position in the matrix  */
	const string KReset   ("0"); /**  \fn \brief String: a type to return to the original color */
	const string KRouge   ("31"); /**  \fn \brief String: a type to set a string in red */
	const string KVert    ("32"); /**  \fn \brief String: a type to set a string in green */
	const string KJaune   ("33"); /**  \fn \brief String: a type to set a string in jaune */
	const string KBleu    ("34"); /**  \fn \brief String: a type to set a string in bleu */
	
	const char KTokenPlayer1 = 'X'; /**  \var \brief Char: a type representing the red player */
	const char KTokenPlayer2 = 'O'; /**  \var \brief Char: a type representing the blue player */
	const char KEmpty        = ' '; /**  \var \brief Char: a type representing empty squares */
	const char KWall         = '#'; /**  \var \brief Char: a type representing walls */
	const char KFood         = 'f'; /**  \var \brief Char: a type representing food */
	unsigned scorebleu=0; /** \var \brief a type representing the blue player score */
	unsigned scorerouge=0; /** \var \brief a type representing the red player score */
	
	/**
	* \fn ClearScreen ()
	* \brief function to clear the display
	*/
	void ClearScreen ()
	{
    		cout << "\033[H\033[2J";
	}//ClearScreen()
	
	/**
	* \fn Couleur (const string & coul)
	* \brief function to change the color of the display.
	*
	*\param coul : Colors 
	*/
	void Couleur (const string & coul)
	{
		cout << "\033[" << coul <<"m";
	}//Couleur()
	
	/**
	* \fn InitMat(CMatrix & Mat, unsigned Msize)
	* \brief function to create the map where the game take place.
	*
	*\param Mat : Matrix
	*\param Msize : Size of the matrix
	*/
	void InitMat(CMatrix & Mat, unsigned Msize)
	{
		srand(time(NULL));
	
		Mat.resize(Msize);
		for(unsigned i=0;i<Msize;++i)
		{
			Mat[i].resize(Msize);
		}
		
		for(unsigned i=0;i<Mat.size();++i)
		{
			for(unsigned j=0;j<Mat.size();++j)
			{
				Mat[i][j]=KEmpty;
			}
		}
		
		//wall generation
		for(unsigned i=0;i<(Mat.size()/3)+1;++i)
		{
			Mat[i][Msize/2]=KWall;
		}
		for(unsigned i=0;i<Mat.size()/3;++i)
		{
			Mat[Msize/4][i]=KWall;
		}
			Mat[1][Msize-2]=KWall;
		for(unsigned i=0;i<Mat.size()/4;++i)
		{
			Mat[Msize/2-1][i]=KWall;
		}
		for(unsigned i=0;i<(Mat.size()/2)+1;++i)
		{
			Mat[Msize-1][i]=KWall;
		}
		Mat[Msize/2][Msize-2]=KWall;
		Mat[Msize/2+1][Msize/2-1]=KWall;
		Mat[Msize/2+1][Msize-2]=KWall;
		Mat[(Msize/2)+1][(Msize/3)-1]=KWall;
		//fin wall generation
		
		for(unsigned i=0;i<Mat.size();++i)
		{
			for(unsigned j=0;j<Mat.size();++j)
			{
				unsigned rando=rand() %8;
				if(rando == 0 && Mat[i][j] != KWall)
				{
					// Mat[i][j]=KFood;
				}
			}
		}
	}//InitMat()
	
	
	
	/**
	* \fn ShowMat (CMatrix & Mat, pair<unsigned,unsigned> PosRouge, pair<unsigned,unsigned> PosBleu)
	* \brief function used to display the matrix who is composed by players, food, walls and empty squares.
	*
	*\param Mat : Matrix
	*\param PosRouge : Red Player position
	*\param PosBleu : Blue Player position
	*/
	void  ShowMat (CMatrix & Mat, pair<unsigned,unsigned> PosP1, pair<unsigned,unsigned> PosP2)
	{
		ClearScreen();
		for(unsigned i=0;i<Mat.size();++i)
		{
			for(unsigned j=0;j<Mat.size();++j)
			{
				if(i==PosP1.first && j==PosP1.second)
					{
					Couleur(KRouge);
					Mat[i][j]=KTokenPlayer1;
					}
				else if(i==PosP2.first && j==PosP2.second)
					{
					Couleur(KBleu);
					Mat[i][j]=KTokenPlayer2;
					}
				else if(Mat[i][j]==KFood)
					{
						Couleur(KJaune);
					}
				else if(Mat[i][j]==KWall)
					{
						Couleur(KVert);
					}
				else
				{
					Couleur(KReset);
				}
				cout<<setw(4)<<'['<<Mat[i][j]<<']';
			}
			Couleur(KReset);
			cout<<endl<<endl<<endl;
		}
	}//ShowMat()
	
	
	/**
	* \fn NoFood(CMatrix & Mat)
	* \brief function to know if there are food.
	*
	*\param Mat : Matrix
	*/
	bool NoFood(CMatrix & Mat)
	{
		unsigned ComptFood = 0;
		for(;;)
		{
			for(unsigned i=0;i<Mat.size();++i)
			{
				for(unsigned j=0;j<Mat.size();++j)
				{
					if (Mat[i][j]==KFood)
						ComptFood = ComptFood+1;
				}
			}
			if (ComptFood==0)
			{
				cout<< "Il n'y a plus de petits pains. Le joueur ";
				Couleur(KRouge);
				cout<< "rouge ";
				Couleur(KReset);
				cout<< "gagne la partie."<<endl<<endl;
				// affichage score
				scorerouge=scorerouge+1;
				cout<<"Score :"<<endl;
				cout<<"Joueur";
				Couleur(KBleu);
				cout<< " bleu  ";
				Couleur(KReset);
				cout<< ":" << scorebleu <<endl;
				cout<<"Joueur";
				Couleur(KRouge);
				cout<< " rouge ";
				Couleur(KReset);
				cout<< ":" << scorerouge <<endl<<endl;
				// affichage options
				cout<<"Tapez Q pour quitter."<<endl;
				cout<<"Tapez R pour recommencer."<<endl;
				return true;
				break;
			}
			else return false;
		}
	}//NoFood()
	
	
	/**
	* \fn MoveToken(CMatrix & Mat,char & Move, CPosition & Pos, unsigned Msize)
	* \brief Function used to move your token in different directions 
	*
	*\param Mat : Matrix
	*\param Move : Movemement
	*\param Pos : Position
	*\param Msize : Size of the matrix
	*
	*Z: The token moves one square up
	*S: The token moves one square down
	*Q: The token moves one space to the left
	*D: The token moves one space to the right
	* Note: If a token attempts to exit the map, an error message appears
	*/
	void MoveToken(CMatrix & Mat,char & Move, CPosition & Pos, unsigned Msize)
	{
		unsigned P1=get<0>(Pos);// ancienne position
		unsigned P2=get<1>(Pos);// ancienne position
		
		while(true)
		{
			cin>>Move;
			Move=toupper(Move);
			switch (Move)
			{
				case 'Z':
						if(Mat[Pos.first] != Mat[0])
						{
							if (Mat[Pos.first-1][Pos.second]!=KWall)
							{
								--Pos.first;
								break;
							}
						}
					else
						cout<<"Erreur, restez dans la carte."<<endl;
						continue;
					
				case 'S':
						if(Mat[Pos.first] != Mat[Msize-1])
						{
							if (Mat[Pos.first+1][Pos.second]!=KWall)
							{
								++Pos.first;
								break;
							}
						}
					else
						cout<<"Erreur, restez dans la carte."<<endl;
						continue;
				case 'Q':
						if(Mat[Pos.first][Pos.second] != Mat[Pos.first][0])
						{
							if (Mat[Pos.first][Pos.second-1]!=KWall)
							{
								--Pos.second;
								break;
							}
						}
					else
						cout<<"Erreur, restez dans la carte."<<endl;
						continue;
				case 'D':
						if(Mat[Pos.first][Pos.second] != Mat[Pos.first][Msize-1])
						{
							if (Mat[Pos.first][Pos.second+1]!=KWall)
							{
								++Pos.second;
								break;
							}
						}
					else
						cout<<"Erreur, restez dans la carte."<<endl;
						continue;
				default:
					cout<<"Erreur, mouvement incorrect."<<endl;
					continue;
			}
			break;
		}
		Mat[P1][P2]=KEmpty;//KEmpty sur l'ancienne position
	}//MoveToken()
	
	/**
	* \fn Menu()
	* \brief function used to display the menu.
	*
	*/
	void Menu()
	{
		ifstream titre;
		titre.open("./titre.txt");
		if(titre) //Si le fichier existe
		{
			while(titre) //Tant qu'on n'est pas a la fin
			{
				string ligne;
				getline(titre, ligne); //On lit une ligne
				Couleur(KJaune);
				cout << ligne << endl; //Et on l'affiche dans la console
				Couleur(KReset);
			}
		}
		else				//Si le fichier n'existe pas
		{
			cout << "ERREUR: Le fichier n'est pas disponible." << endl;
		}
		cout<<"Bienvenue gamer, tu es sur le point de commencer une partie de \"CATCH ME IF YOU CAN !\" avec un autre être humain !"<<endl<<endl;
		
		ifstream menu;
		menu.open("./menu.txt");
		if(menu)
		{
			while(menu)
			{
				string ligne;
				getline(menu, ligne);
				cout << ligne << endl;
			}
		}
		else
		{
			cout << "ERREUR: Le fichier texte contenant le menu n'est pas disponible." << endl;
		}
	}// Menu()
	
	/**
	* \fn int Game(void)
	* \brief function of the game.
	*
	*/
	int Game(void)
	{
		CMatrix Mat;
		unsigned Msize;
		CPosition PosP1,PosP2;
		char Move;
		ifstream config;
		config.open("./config.txt");
		if(config)
		{
			string ligne;
			unsigned temp;
			unsigned temp2;
			config>>ligne>>temp;
			cout<<temp<<'*';
			Msize=temp;//taille matrice du fichier
			config>>ligne>>temp;
			config>>ligne>>temp2;
			if(temp==Msize || temp2==Msize)
			//on peut ecrire postion 4 en prenant en compte que le vector commence a l'indice 0 ou on peut dire aussi 5 si l'on ne prend pas en compte que le vector commence a l'indice 0
			{
				temp=temp-1;
				temp2=temp2-1;
			}
			PosP1=make_pair(temp,temp2);
			config>>ligne>>temp;
			config>>ligne>>temp2;
			if(temp==Msize || temp2==Msize)
			//on peut ecrire postion 4 en prenant en compte que le vector commence a l'indice 0 ou on peut dire aussi 5 si l'on ne prend pas en compte que le vector commence a l'indice 0
			{
				temp=temp-1;
				temp2=temp2-1;
			}
			PosP2=make_pair(temp,temp2);
		}
		else
		{
			cout << "ERREUR: Le fichier config.txt n'est pas disponible." << endl;
		}
		
		InitMat(Mat,Msize);
		ShowMat(Mat,PosP1,PosP2);
		while(true)
		{
			//joueur rouge
			if(NoFood(Mat) == true) break;
			cout<<endl<<"Deplacez-vous joueur ";
			Couleur(KRouge);
			cout<<"rouge";
			Couleur(KReset);
			cout<<':'<<'\n';
			
			MoveToken(Mat,Move,PosP1,Msize);
			ShowMat(Mat,PosP1,PosP2);
			
			// si les deux joueurs sont sur la même case, alors le joueur bleu gagne la partie
			if (PosP1==PosP2)
			{
				cout<< "Le joueur ";
				Couleur(KBleu);
				cout<< "bleu ";
				Couleur(KReset);
				cout<< "gagne la partie."<<endl<<endl;
				// affichage score
				scorebleu=scorebleu+1;
				cout<<"Score :"<<endl;
				cout<<"Joueur";
				Couleur(KBleu);
				cout<< " bleu ";
				Couleur(KReset);
				cout<< ":" << scorebleu <<endl;
				cout<<"Joueur";
				Couleur(KRouge);
				cout<< " rouge ";
				Couleur(KReset);
				cout<< ":" << scorerouge <<endl<<endl;
				// affichage options
				cout<<"Tapez Q pour quitter."<<endl;
				cout<<"Tapez R pour recommencer."<<endl;
				break;
			}
			
			//joueur bleu
			if(NoFood(Mat) == true) break;
			cout<<endl<<"Deplacez-vous joueur ";
			Couleur(KBleu);
			cout<<"bleu";
			Couleur(KReset);
			cout<<':'<<'\n';
			
			MoveToken(Mat,Move,PosP2,Msize);
			ShowMat(Mat,PosP1,PosP2);
			
			if (PosP1==PosP2)
			{
				cout<< "Le joueur ";
				Couleur(KBleu);
				cout<< "bleu  ";
				Couleur(KReset);
				cout<< "gagne la partie."<<endl<<endl;
				// affichage score
				scorebleu=scorebleu+1;
				cout<<"Score :"<<endl;
				cout<<"Joueur";
				Couleur(KBleu);
				cout<< " bleu  ";
				Couleur(KReset);
				cout<< ":" << scorebleu <<endl;
				cout<<"Joueur";
				Couleur(KRouge);
				cout<< " rouge ";
				Couleur(KReset);
				cout<< ":" << scorerouge <<endl<<endl;
				// affichage options
				cout<<"Tapez Q pour quitter."<<endl;
				cout<<"Tapez R pour recommencer."<<endl;
				break;
			}
		}
		return 0;
	}//game()
	
	
	/**
	 * \fn int Run(void)
	 * \brief main function that runs when launching the game
	 *
	 * \return 0 - Normal program termination.
	 */
	int Run(void)
	{
		char NavMenu;
		ifstream regles;
		ifstream credit;
		while(true)
		{
			ClearScreen();
			Menu();
			cin>>NavMenu;
			switch (NavMenu)
			{
				case '1':
					Game();
					char option;
					while(true)
					{
						cin>>option;
						option=toupper(option);
						if(option=='Q')
							break;
						else if(option=='R')
							Game();
					}
					ClearScreen();
					break;

				case '2':
					ClearScreen();
					regles.open("./regles.txt");
					if(regles)			//Si le fichier existe
					{
						while(regles)    //Tant qu'on n'est pas a la fin
						{
						string ligne;
						getline(regles, ligne); //On lit une ligne
						cout << ligne << endl; //Et on l'affiche dans la console
						}
						regles.close();
						while(true)
						{
							cin>>NavMenu;
							NavMenu=toupper(NavMenu);
							if(NavMenu=='M')
							{
								ClearScreen();
								break;
							}
							else
								continue;
						}
						continue;
					}
					else				//Si le fichier n'existe pas
					{
						cout << "ERREUR: Le fichier n'est pas disponible." << endl;
						break;
					}
						
				case '3':
					ClearScreen();
					credit.open("./credit.txt");
					if(credit) //Si le fichier existe
					{
						while(credit) //Tant qu'on n'est pas a la fin
						{
							string ligne;
							getline(credit, ligne); //On lit une ligne
							cout << ligne << endl; //Et on l'affiche dans la console
						}
						credit.close();
						cout<<endl<<"Tapez M pour revenir au menu."<<endl;
						while(true)
						{
							cin>>NavMenu;
							NavMenu=toupper(NavMenu);
							if(NavMenu=='M')
							{
								ClearScreen();
								break;
							}
							else
								continue;
						}
						continue;
					}
					
					else				//Si le fichier n'existe pas
					{
						ClearScreen();
						cout << "ERREUR: Le fichier n'est pas disponible." << endl;
						break;
					}
					
				case '4':
					ClearScreen();
					break;
				default:
					cout<<"Erreur, saisissez une commande valable. (1,2,3,4)"<<endl;
					continue;
			}
			break;
		}
		return 0;
	}
}

/**
 * \fn int main()
 * \brief Program input.
 *
 * \return 1 - Program Bug.
 * \return 0 - Normal program termination.
 */
int main()
{
	try
{
	return catch_me::Run();
}
	catch(...)
{
	cerr<<"bug"<<endl;
	return 1;
}
return 0;

}//int main()

#include <iostream>

#include <cstring>
#include <fstream>

#include "node.h"


using namespace std;

//by ryan veith
//binary search tree
//should be able to add, remove and print in any order
//nodes can have anything from 1 to 999
//seperate inputs with spaces for add by file

//Working on making it a balanced tree with red black
//added color (black = 0 red = 1)
//code got really complex and just unreadable with adding so ripped it out and
//tried to make rotates and insertRebalance simple
//and put comments down there on how they work

//40 30 20 50 45 Bug from insert is fixed
//it was breaking becasue of a commented out rotate
// ...that had a non-commented cout above it saying rotate.
//so yeah that happend

//Otherwise as far as I can tell deletion works properly
//for deletion it find the node and then before deleting it it reshifts tree
//for balancing then continues deleting it

//There is also a search funcion

void print(Node* current, int tab);
//print out the tree in a way that lets you see children + parents + color
void printTest(Node* current, int tab, bool color);
//testprint can show less

//I got bored
void check(Node* current, Node* previous);

void add(Node* &root, Node* current, Node* previous, int thing);
//add thing to the binary tree

void remove(Node* &root, Node* current, Node* previous,int thing);
//three cases here (one is two children, one is one child, one is none)
//also I am handeling root differntlu since pas by refferece

bool search(Node* current, int thing);
//ture is found it, false is not found

//maintian red-black tree properties after problems
void insertRebalance(Node* newNode, Node* &root);
void deleteRebalance(Node* k, Node* &root);

//I wanted something to call when a node was deleted from the tree to make
//what is happpening easier to understand
void deleteAlert(Node* toDelete, Node* &root);

//manipulating tree
void leftRotate(Node* x, Node* &root);
void rightRotate(Node* x, Node* &root);

int main() {
  //cout << "hello world" << endl;
  cout << "Type HELP for a list of commands" << endl;
  bool quit = false;
  char input[80];
  Node* root = NULL;
  //main loop of program for user input
  while(quit == false) {
    cout << "checkcall" << endl;
    check(root, NULL);
    cout << "endcheck" << endl;
    
    //ask user input
    cin.getline(input, 80);
    if (strcmp(input, "QUIT") == 0) {
      quit = true;
    }
    if (strcmp(input, "HELP") == 0) {
      cout << "Commands are:" << endl;
      cout << "PRINT   *PRINT1 and PRINT2 give different info" << endl;
      cout << "ADD" << endl;
      cout << "REMOVE" << endl;
      cout << "SEARCH" << endl;
      cout << "QUIT" << endl;
    }
    //multiple prints with varying levels of info
    if (strcmp(input, "PRINT") == 0) {
      cout << "Binary Search Tree:" << endl;
      print(root, 0);
    }
    if (strcmp(input, "PRINT1") == 0) {
      cout << "Binary Search Tree(parentless):" << endl;
      printTest(root, 0, true);
    }
    if (strcmp(input, "PRINT2") == 0) {
      cout << "Binary Search Tree(basic):" << endl;
      printTest(root, 0, false);
    }
    if (strcmp(input, "REMOVE") == 0) {
      int thing;
      cout << "what number would you like to remove?" << endl;
      cin >> thing;
      remove(root, root,  NULL, thing);
    }
    if (strcmp(input, "SEARCH") == 0) {
      int thing;
      cout << "what number would you like to searfch for?" << endl;
      cin >> thing;
      if (search(root, thing) == true) {
	cout << thing << " is in the tree" << endl;
      }
      else {
	cout << thing << " is not in the tree" << endl;
      }
    }
    if (strcmp(input, "ADD") == 0) {
      //file vs console
      char input;
      cout << "Would you like to add from a file (1) ";
      cout << "or the console (2)?" << endl;
      cin >> input;
      if (input == '1') {
	//file handeling
	cout << "What is the name of the file?" << endl;
        char word[80];
	cin >> word;
	//open file and go though it
	ifstream myFile (word);
	if (myFile.is_open()) {
	  //while so keep going until hit end
	  bool notQuit = true;
	  while (notQuit) {
	    //cin number
	    int t = -1;
	    myFile >> t;
	    //cout << "t: " << t << endl;
	    //file will stop overriding once it hits the end
	    if (t >= 0) {
	      //add it
	      add(root, root, NULL, t);
	    }
	    else {
	      notQuit = false;;
	    }
	  }
	}
	else {
	  cout << "something went worng, ";
	  cout << "please check your spelling of the file name" << endl;
	}
	myFile.close();
	cout << "done" << endl;;
      }
      else if (input == '2') {
	cout << "Enter numbers seperated by spaces" << endl;
	//input handling
	int thing;
	cin >> thing;
	add(root, root, NULL, thing);
      }
    }
  }
}

void check(Node* current, Node* previous) {
  //cout << "check" << endl;
  //this is the I got bored code
  //It sould go make sure tree is ok (double linked)
  if(previous == NULL && current != NULL) {
    check(current->getRight(), current);
    check(current->getLeft(), current);
  }
  else if(current != NULL) {
    //update parent
    if(current == previous->getRight()) {
      current->setParent(previous);
    }
    else if(current == previous->getLeft()) {
      current->setParent(previous);
    }

    //recurse
    check(current->getRight(), current);
    check(current->getLeft(), current);
  }
}


//a fun program I made back in hashtable simplified for a binary tree
void print(Node* current, int tab) {
  if (current != NULL) {
    //cout << "print: " << current->getNumber() << endl;
    //call print on left node
    print(current->getLeft(), tab + 1);
    //print myself with correct tab number
    for (int a = 0; a < tab; a++) {
      cout << "    ";
    }
    //cout a R or B before node for color
    if (current->getColor() == 0) {
      cout << "B";
    }
    else if (current->getColor() == 1) {
      cout << "R";
    }
    //cout #
    cout << current->getNumber();
    //cout parrent
    if (current->getParent() != NULL) {
      cout << " p=" << current->getParent()->getNumber();
    }
    else {
      cout << " p=NULL";
    }
    //end line
    cout << endl;
    
    //call print on right node
    print(current->getRight(), tab + 1);
  }
}

//this prints without showing parent
void printTest(Node* current, int tab, bool color) {
  if (current != NULL) {
    //cout << "print: " << current->getNumber() << endl;
    //call print on left node
    printTest(current->getLeft(), tab + 1, color);
    //print myself with correct tab number
    for (int a = 0; a < tab; a++) {
      cout << "    ";
    }
    if(color == true) {
      //cout a R or B before node for color
      if (current->getColor() == 0) {
	cout << "B";
      }
      else if (current->getColor() == 1) {
	cout << "R";
      }
    }
    //cout #
    cout << current->getNumber();
    //end line
    cout << endl;
    
    //call print on right node
    printTest(current->getRight(), tab + 1, color);
  }
}

//I don't think I will ever need to use previous
//but easy to have for if needed
void add(Node* &root, Node* current, Node* previous, int thing) {
  //first check if it is a legal int to add
  //(does it appear in the tree aready)
  if (search(root, thing) == true) {
    cout << thing << " is aready in the tree" << endl;
    cout << "You may not add duplicate numbers to the tree" << endl;
    return;
  }

  //Do the add fuction
  //do we need to replace head?
  if(root == NULL) {
    Node* newNode = new Node(thing);
    root = newNode;
    //root parent = NULL by default
  }
  else if(current == NULL) {
    cout << "something went wrong" << endl;
  }
  ///if bigger go right
  else if (current->getNumber() > thing) {
    if (current->getRight() != NULL) {
      //go there
      add(root, current->getRight(), current, thing);
    }
    else {
      //add me there
      Node* newNode = new Node(thing);
      current->setRight(newNode);

      //added a Node to the Tree!!! handle balancing
      //list parent
      newNode->setParent(current);
      //new node is red by default
      newNode->setColor(1);
      //figure out balencing
      insertRebalance(newNode, root);
      
    }
  }
  //if we are smaller go left
  else if (current->getNumber() < thing) {
    if (current->getLeft() != NULL) {
      //go there
      add(root, current->getLeft(), current, thing);
    }
    else {
      //add me there
      Node* newNode = new Node(thing);
      current->setLeft(newNode);

      
      //added a Node to the Tree!!! handle balancing
      //same code as above!!!!!
      //list parent
      newNode->setParent(current);
      //new node is red by default
      newNode->setColor(1);
      //figure out balencing
      insertRebalance(newNode, root);
    }
  }
  else {
    //duplicats are not allowed
    cout << "Um..." << endl;
  }
}
void remove(Node* &root, Node* current, Node* previous, int thing) {
  cout << "problem root: " << root->getNumber() << endl;
  
  if (previous != NULL) {
    //I just want to know if this was left or right node of previous
    //this is not very useful but...
    bool right = true;
    if (previous->getNumber() > thing) {
      right = true;
    }
    else {
      right = false;
    }
  }
      
  //three cases here (one is root)
  //kindof, I am also treating head = null as seperate since by reference
  if(current == NULL) {
    cout << "Nothing to remove " << endl;
  }
  else if(root->getNumber() == thing) {
    //remove current, current = root
    cout << "remove current, current = NULL" << endl;

    //if this is the only thing in the tree
    if(current->getRight() == NULL && current->getLeft() == NULL) {
      //just delete this one
      cout << "empty tree" << endl;
      root = NULL;
      delete current;

      //no rebalancing needed deleted the root of the tree
    }

    //if there is one node
    //it becomes root
    else if(current->getRight() != NULL && current->getLeft() == NULL) {
      cout << "root replaced" << endl;
      root = current->getRight();
      //root->parent is always null
      root->setParent(NULL);
      delete current;

      //the new root of the tree is black
      root->setColor(0);
    }
    //if there is one node (otherside)
    else if(current->getLeft() != NULL  && current->getRight() == NULL) {
      root = current->getLeft();
      cout << "root replaced" << endl;
      //root->parent is always null
      root->setParent(NULL);
      delete current;

      //the new root of the tree is black
      root->setColor(0);

    }
    
    else if(current->getLeft() != NULL  && current->getRight() != NULL) {
      //two chidren
      cout << "two children" << endl;
      //go right once, then left untill getLeft() = NULL
      //delete that one
      //then replace me with them
      Node* temp = current->getRight();
      while (temp->getLeft() != NULL) {
	temp = temp->getLeft();
      }
      int replace = temp->getNumber();

      //don't swap colors  Do NOT
      /*
      int newColor = temp->getColor();
      temp->setColor(current->getColor());
      current->setColor(newColor);
      //*/

      //call delete on the last one
      remove(root, root, NULL, replace);
      
      //set current to a copy of the last one
      current->setNumber(replace);
      //update parent
      current->setParent(temp->getParent());
      
    }
    cout << "done with root" << endl;
  }
  else if (current->getNumber() == thing) {
    //remove current thing somehow
    cout << "target: " << thing << ", current: " << current->getNumber() << endl;
    
    //if this is the end (leaf)
    if(current->getRight() == NULL && current->getLeft() == NULL) {
      //just delete this one

      //the new plan is to call alert before deleting so do that
      cout << "root: " << root->getNumber() << endl;
      deleteAlert(current, root);
      //sucessor is currently not bieng passed (its the NULL)
      //side note. we are not doing this. search will be in alert
      //first that will then call this later

      
      if (previous->getNumber() > thing) {
	//right
	previous->setRight(NULL);
      }
      else {
	//left
	previous->setLeft(NULL);
      }

      //print(root, 0);
      //something was deleted call the alert
      //deleteAlert(current, NULL, previous, root);

      delete current;
    }

    //if there is one node
    //we move the that one up
    //it will work because of how they were added
    
    
    else if(current->getRight() != NULL && current->getLeft() == NULL) {
      //call alert first
      cout << "root: " << root->getNumber() << endl;
      deleteAlert(current, root);

      Node* successor = NULL;
      if (previous->getNumber() > thing) {
	//right
	previous->setRight(current->getRight());
	//update parent
	current->getRight()->setParent(previous);

	 successor = current->getRight();
      }
      else {
	//left
	previous->setLeft(current->getRight());
	//update parent
	current->getRight()->setParent(previous);

        successor = current->getRight();
      }

      //something was deleted call the alert
    }
    //if there is one node (otherside)
    else if(current->getLeft() != NULL  && current->getRight() == NULL) {
      //call delete alert first
      cout << "root: " << root->getNumber() << endl;
      deleteAlert(current, root);

      Node* successor = NULL;
      if (previous->getNumber() > thing) {
	//right
	previous->setRight(current->getLeft());
	//update parent
	current->getLeft()->setParent(previous);

        successor = current->getLeft();
      }
      else {
	//left
	previous->setLeft(current->getLeft());
	//update parent
	current->getLeft()->setParent(previous);

        successor = current->getLeft();
      }

      //something was deleted call the alert
    }

    else if(current->getLeft() != NULL  && current->getRight() != NULL) {
      //two chidren
      //go right once, then left untill getLeft() = NULL
      //delete that one
      //then replace me with them
      Node* temp = current->getRight();
      while (temp->getLeft() != NULL) {
	temp = temp->getLeft();
      }
      int replace = temp->getNumber();

      //swap colors exept I don;t think you do
      /*
      int newColor = temp->getColor();
      temp->setColor(current->getColor());
      current->setColor(newColor);
      //*/

      //even if root = current there shoudl be no reason it can be
      //root, current , Null , replace (vs) c c, n ,r
      remove(root, current, NULL, replace);
      //cout << "done" << endl;
      current->setNumber(replace);
      
      //update parent
      current->setParent(temp->getParent());
      
    }
    else {
      cout << "We can't remove that" << endl;
    }
    
  }
  //this is not the thing to delete so go find that thing
  else if (current->getNumber() > thing) {
    //cout << "right" << endl;
    remove(root, current->getRight(), current, thing);
  }
  else {
    //cout << "left" << endl;
    remove(root, current->getLeft(), current, thing);
  }

  //cout << "problem root: " << root->getNumber() << endl;
}
bool search(Node* current, int thing) {
  //possibly retruns
  if (current == NULL) {
    return false;
  }
  else if(current->getNumber() == thing) {
    return true;
  }
  else {
    bool a = search(current->getRight(), thing);
    bool b = search(current->getLeft(), thing);
    if (a == true || b == true) {
      return true;
    }
    else {
      return false;
    }
  }
}

//uncle can be null (i think atleast)
//also most of the anit seg fault is reduntant but at this point I keep readding
//impssible seg fault situations so it is in for next time
void insertRebalance(Node* newNode, Node* &root) {
  cout << "root: " << root->getNumber() << endl;
  
  cout << "insertRebalence" << endl;
  Node* k = newNode;
  //while loop since you cant breack rule if your parent is black
  //also double checks things as k changes

  print(root, 0);
  cout << k->getParent()->getColor() << endl;
  bool loop = false;
  if (k->getParent()->getColor() == 1) {
    loop = true;
  }
  
  while (loop == true) { //red (or has case where quits if it is root)
    if (k->getParent()->getParent() != NULL) {

      /*
      Node* gp = k->getParent()->getParent();
      if (gp != NULL) {
	cout << "gp: " << gp->getNumber() << endl;
      } 
      if (gp->getRight() != NULL) {
	cout << "r: " << gp->getRight()->getNumber() << endl;
      }
      
      if (gp->getLeft() != NULL) {
	cout << "l: " << gp->getLeft()->getNumber() << endl;
      }
      //*/

      
      if (k->getParent() == k->getParent()->getParent()->getRight()) {

	//cout << "f" << endl;
	//save 'uncle' node
	//with anti seg fault measures
	Node* u = NULL;
	if (k->getParent() != NULL) {
	  if (k->getParent()->getParent() != NULL) {
	    u = k->getParent()->getParent()->getLeft();
	  }
	}
	//cout << "hello" << endl;
	//bool check = false;
	//seg fault layer
	//if(u != NULL) {
	//cout << "uncle: "<< u->getNumber() << endl;
	//print(root, 0);
	int tempcolor = 0;
	if (u != NULL) { 
	  if (u->getColor() == 1) {
	    tempcolor = 1;
	    cout << "tempcolor" << endl;
	  }
	}
	cout << "made it to if" << endl;
	
	if (tempcolor  == 1) { //red, case 3.1
	  cout << "this is happening" << endl;
	  u->setColor(0); //black
	  k->getParent()->setColor(0);
	  k->getParent()->getParent()->setColor(1); //gp -> red
	  k = k->getParent()->getParent(); //change k
	  //print(root, 0);
	  //check = true;
	  print(root, 0);
	    
	}
	
	//}
	//cout << check << endl;
	//dont do if check??
	
        else if (k == k->getParent()->getLeft()) { //case 3.31 and 3.32
	    cout << "we are doing the else if" << endl;
	    
	    k = k->getParent();
	    //!left rotate
	    cout << "!left rotate" << endl;
	    rightRotate(k, root);
	    print(root,0);
	    cout << "done" << endl;
	    //check = true;
	  }
	
	
	//cout << "hi" << endl;
	//print(root, 0);

	//cout <<k->getParent()->getRight() << "!!!" << endl;
	//cout << k->getParent()->getNumber();
	
	//tree can be balcenced earlier so need to check
	else {
	  cout << "else" << endl;
	  //if (check == false) { //parent is not red
	  k->getParent()->setColor(0); //black
	  k->getParent()->getParent()->setColor(1); //gp -> red
	  //!right rotate
	  cout << "!right rotate" << endl;
	  cout<<"rotating: "<< k->getParent()->getParent()->getNumber() << endl;
	  leftRotate(k->getParent()->getParent(), root); //gp
	  print(root,0);
	  cout << "done" << endl;
	}
	cout << "lp" << endl;
      }
      else {
	//do same things with left and right swapped 3.3.3 and 3.3.4
	cout << "mirror" << endl;
	if (k->getParent() == k->getParent()->getParent()->getLeft()) {
	  //save 'uncle' node
	  Node* u = NULL;
	  if (k->getParent() != NULL) {
	    if (k->getParent()->getParent() != NULL) {
	      u = k->getParent()->getParent()->getRight();
	    }
	  }
	  int tempcolor = 0;
	  //seg fault layer
	  if(u != NULL) {
	    if (u->getColor() == 1) {
	      tempcolor = 1;
	      cout << tempcolor << endl;
	    }
	  }

	  if (tempcolor  == 1) { //red, case 3.1
	    cout << "push blackness down" << endl;
	    u->setColor(0); //black
	    k->getParent()->setColor(0);
	    k->getParent()->getParent()->setColor(1); //gp -> red
	    k = k->getParent()->getParent(); //change k
	  }
	  else if (k == k->getParent()->getRight()) { //case 3.31 and 3.32
	    k = k->getParent();
	    //left rotate
	    cout << "left rotate" << endl;
	    leftRotate(k, root);
	    print(root,0);
	    cout << "done" << endl;
	    }

	  //check we did not balence it aready
	  else {
	    k->getParent()->setColor(0); //black
	    k->getParent()->getParent()->setColor(1); //gp -> red
	    //right rotate
	    //print(root,0);
	    cout << "right rotate" << endl;
	    rightRotate(k->getParent()->getParent(), root); //gp
	    //print(root,0);
	    cout << "done" << endl;
	  }
	}
      }
    }
    else {
      //???
      //me having fun:
      cout << "what did you do?" << endl;
      cout << "..." <<  endl;
      cout << endl;
      cout << "you should always be able to go to parent" << endl;
      cout << "and going up twice should not hit root of tree" << endl;
      cout << "really what did you manage to break" << endl;      
      exit(1);
    }




    //a case exsts where k can be root so k.getparent.getcolor is seg fault
    //soo this exists and is a boolean it checs so that update can be in 2 ifs
    //cout << k->getNumber() << endl;
    if (k->getParent() == NULL) {
      loop = false;
    }
    else if(k->getParent()->getColor() != 1) {
      loop = false;
    }

    
  } //end of while
  root->setColor(0); //root is allways black
}

void deleteAlert(Node* toDelete, Node* &root) {
  //for updated version call this just before deleting a node, wait
  //just search for node in this "rebalnace" so the delete is now ok
  //then call delete?
  cout << "ALERT" << endl;
  cout << "root: " << root->getNumber() << endl;
  cout << toDelete->getNumber() << endl;
  //exit(1);
  bool rebalance = true;
  if (toDelete == NULL) {
    rebalance = false;
  }
  else if (toDelete->getColor() == 1) {
    //dont need to rebalance
  }
  else {
    //there is a case where to delete is black but has a solo red child
    //deal with that now
    if (toDelete->getRight() == NULL) {
      if (toDelete->getLeft() != NULL) {
	if (toDelete->getLeft()->getColor() == 1) {
	  //child become black
	  toDelete->getLeft()->setColor(0);
	  //and go straight to deletion of to delete
	  rebalance = false;
	  cout << "skip balence" << endl;
	}
      }
    }
    if (toDelete->getLeft() == NULL) {
      if (toDelete->getRight() != NULL) {
	if (toDelete->getRight()->getColor() == 1) {
	  //child become black
	  toDelete->getRight()->setColor(0);
	  //and go straight to deletion of to delete
	  rebalance = false;
	  cout << "skip balence" << endl;
	}
      }
    }
  }
  
  //call delete rebalancing
  if (rebalance == true) {
    deleteRebalance(toDelete, root);
  }
    
  //end by going back to delte which will delete the node
  //(it might botch the delete) need to probably redo that

  cout << "DONE" << endl;
  cout << "root: " << root->getNumber() << endl;
}

void deleteRebalance(Node* x, Node* &root) {
  //rewriting this so  it is better symetry

  //sibling
  Node* s = NULL;
  cout << "rebalancing" << endl;
  cout << "root = " << root->getNumber() << ", or: " << root << endl;

  while (x != root && x->getColor() == 0) {
    //do one side so it is just mirroring
    //might need to add checks for NULL, like even more somehow
    if (x == x->getParent()->getLeft()) {
      cout << "if" << endl;
      //set sibling for future reference
      s = x->getParent()->getRight();
      //this can be null so yeah

      bool blackSibling = true;
      if (s != NULL) {
	if (s->getColor() == 1) {
	  blackSibling = false;
	}
      }

      //as long as s is not NULL
      if(blackSibling == false) {
	s->setColor(0);
	x->getParent()->setColor(1);
	leftRotate(x->getParent(), root);

	//overwrite it or not???
	s = x->getParent()->getRight();
      }

      bool pass = true;
      if (s != NULL) {
	if(s->getLeft() != NULL) {
	  if(s->getLeft()->getColor() == 1) {
	    pass = false;
	  }
	}
	if(s->getRight() != NULL) {
	  if(s->getRight()->getColor() == 1) {
	    pass = false;
	  }
	}
      }
      else {
	//if s in null make pass false
	//pass = false;
      }

      cout << "pass: " << pass << endl;
	    
      //Null nodes are black but will cause seg fault
      //if (s->getLeft()->getColor() == 0 && s->getRight()->getColor() == 0)
      if (pass == true) {
	//make s red and set x to parent
	s->setColor(1);

	//not tested
	//x->getParent()->setColor(0);
	
	x = x->getParent();
      }
      else {
	//one (or both) of s children is red
	//original code rewritten for +seg fault resistance
	//if(s->getRight()->getColor() == 0) {
	if(s->getLeft()!= NULL) {
	  //seg fault barrier
	  if(s->getLeft()->getColor() == 1) {
	    s->getLeft()->setColor(0);
	    s->setColor(1);
	    rightRotate(s, root);
	    //agian on the override or not?
	    s = x->getParent()->getRight();
	  }
	}
	s->setColor(x->getParent()->getColor());
	x->getParent()->setColor(0);
	s->getRight()->setColor(0);
	leftRotate(x->getParent(), root);

	//agian
	x = root;
      }
    }
    else {
      //mirror
      cout << "else" << endl;
      //copy pase then flip

      //set sibling for future reference
      s = x->getParent()->getLeft();
      cout << "s: " << s << endl;
      cout << "x: " << x->getNumber() << endl;
      cout << "P: " << x->getParent()->getNumber() << endl;
      //cout


      //this can be null so yeah

      bool blackSibling = true;
      if (s != NULL) {
	if (s->getColor() == 1) {
	  blackSibling = false;
	}
      }

      //as long as s is not NULL
      //if sibling is red
      if(blackSibling == false) {

	cout << "blacksibling = false" << endl;
	
	s->setColor(0);
	x->getParent()->setColor(1);
	//right not left this time
	rightRotate(x->getParent(), root);

	cout <<"x: " << x->getNumber() << endl;
	//overwrite it or not???
	s = x->getParent()->getLeft();
	cout << "s: " << s << endl;
      }

      bool pass = true;
      if (s != NULL) {
	if(s->getLeft() != NULL) {
	  if(s->getLeft()->getColor() == 1) {
	    pass = false;
	  }
	}
	if(s->getRight() != NULL) {
	  if(s->getRight()->getColor() == 1) {
	    pass = false;
	  }
	}
      }
      else {
	//if s in null make pass false
	//pass = false;
      }
      
      cout << "pass: " << pass << endl;
      print(root, 0);
      //cout << root->getRight()->getNumber() << endl;
      
      //I think I am missing a case where we deal with this since in visulizer
      //it pushes blackness up a level en then see that right nephew is red
      //with a black sibling

      //on second thought that might be true but if x become x->parent
      //so I am not sure it is looping properly
      //might be becase x->starts at 1 (since 2 has two childen)
      //and does not go up propperly?
      
      //Null nodes are black but will cause seg fault
      //if (s->getLeft()->getColor() == 0 && s->getRight()->getColor() == 0)
      if (pass == true) {
	cout << "hello??? " << endl;
	//cout << s << endl;
	//cout << s->getNumber() << endl;
	//make s red and set x to parent
	//print(root, 0);
	cout << "S: " << endl;
	cout << s->getNumber() << endl;
	
	s->setColor(1);

	cout << "x " << x->getNumber() << endl;
	x = x->getParent();
	cout << "x " << x->getNumber() << endl;
	//print(root, 0);
      }
      else {
	//couts for me fixing my mirrorign
	/*
	cout << "?" << endl;
	cout << "s: " << s->getNumber() << endl;
	cout << s->getRight()->getNumber() << endl;
	cout << s->getLeft() << endl;
	cout << s->getLeft()->getNumber() << endl;
	cout << "what?" << endl;
	//*/
	
	//one (or both) of s children is red
	
	//I think this is supposed to be right
	//since this is a rare case when it shoudl not be swapped
	//when mirroring
	cout << "..." << endl;
	//cout << s->getLeft()->getColor() << endl;
	cout << "mark" << endl;

	//this is a more messy way of added the if right exists around the right -> color
	//you can do it simpler since if you look at the red one and it's null its not red so no if
	bool temp = false;
	if(s->getRight() != NULL) {
	  if(s->getRight()->getColor() != 0) {
	    temp = true;
	  }
	}
	if (temp == true) {
	  cout << "hi???" << endl;
	  //on remove 110, form failure it did not do the double
	  //linking correctly somehow 105->parent = 105
	  
	  s->getRight()->setColor(0);
	  s->setColor(1);

	  cout << "left rotate incoming" << endl;
	  print(root, 0);
	  
	  leftRotate(s, root);
	  //agian on the override or not?
	  s = x->getParent()->getLeft();
	}
	cout << "..." << endl;
	s->setColor(x->getParent()->getColor());
	x->getParent()->setColor(0);
	s->getLeft()->setColor(0);

	cout << "printtime" << endl;
	print(root, 0);

	
        rightRotate(x->getParent(), root);

	//agian
	x = root;
      }

      cout << x;
      cout << "x: " << x->getNumber() << endl;
      cout << "color " << x->getColor() << endl;
    }
  }
  cout << "x = " << x->getNumber() << endl;
  x->setColor(0);
  
  if (x != root && x->getColor() == 0) {
    //cout << "THIS IS BROKEN !?!" << endl;
  }
  cout << "x: " << x << " root: " << root << endl;
  cout << "x.color = " << x->getColor() << endl;
  cout << endl;
  
  cout << "x = " << x << endl;
  cout << "# = " << x->getNumber() << endl;
  
  //print (k, 0);


  /*
  
  if (k->getColor() == 1) {
    //just delete it (handeled at end)
  }
  else if (k == root) {
    //same thing
  }
  else {
    //while it is not red or root
    while (k->getColor() == 0 && k != root) {
      cout << "while loop" << endl;

      //set sibling
      Node* sibling = NULL;
      if (k->getParent()->getRight() != k){
	sibling = k->getParent()->getRight();
      }
      else {
	k->getParent()->getLeft();
      }
      bool blackS = true;
      if (sibling != NULL) {
	if (sibling->getColor() == 1) {
	  blackS = false;
	}
      }

      

      //now we need ot figure out what to do
      /*
      //if sibling and siblings children are black (null counts)
      if (sibling != NULL) {
	if(sibling->getRight()
      }
      ///

      if (blackS = false) {
	//swap k.color with sibling.color
	//rotate parent in direction of k
	//loop to start of while (if else'es)
      }

      //one of siblings children is red and other is black
      //(different cases for each)
    }
  }
  //*/
}

//manipulating tree
void leftRotate(Node* x, Node* &root) { //& root or whatever
  //for a left rotation y is the right child of x
  Node* y = x->getRight();
   
  //1st thing
  //make x point to the left subtree of y
  x->setRight(y->getLeft());
  if(x->getRight() != NULL) {
    x->getRight()->setParent(x);
  }
  //2nd thing
  //make x.parent point to y instead
  if (x->getParent() != NULL) {
    //y replaces x in tree
    if (x->getParent()->getRight() == x) {
      x->getParent()->setRight(y);
    }
    else {
      x->getParent()->setLeft(y);
    }
    //double linked
    //I think it is x.parent not x
    y->setParent(x->getParent());
  }
  else {
    //x is root so make y root
    root = y;
    y->setParent(NULL);
  }
  //3rd thing
  //make y.left point to x
  y->setLeft(x);
  x->setParent(y);
}

void rightRotate(Node* x, Node* &root) {
  //for a right rotation y is the left  child of x
  Node* y = x->getLeft();

  //1st thing
  //make x point to the right subtree of y
  x->setLeft(y->getRight());
  if(x->getLeft() != NULL) {
    x->getLeft()->setParent(x);
  }
  //2nd thing
  //make x.parent point to y instead
  if (x->getParent() != NULL) {
    //y replaces x in tree
    if (x->getParent()->getRight() == x) {
      x->getParent()->setRight(y);
    }
    else {
      x->getParent()->setLeft(y);
    }
    //double linked
    y->setParent(x->getParent());
    //cout << y->getNumber() << ", " << y->getParent()->getNumber() << " endl" << endl;
    //cout << x->getNumber() << ", " << x->getParent()->getNumber() << " endl" << endl;
  }
  else {
    //x is root so make y root
    root = y;
    y->setParent(NULL);
  }
  //3rd thing
  //make y.left point to x
  y->setRight(x);
  x->setParent(y);
}

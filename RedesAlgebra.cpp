#include "Attribute.h"			// implementation of attribute types
#include "Algebra.h"			// definition of the algebra
#include "NestedList.h"			// required at many places
#include "QueryProcessor.h"		// needed for implementing value mappings
#include "AlgebraManager.h"		// e.g., check for certain kind
#include "Operator.h"			// for operator creation
#include "StandardTypes.h"		// provides int, real, string, bool type
#include "FTextAlgebra.h"		//
#include "Symbols.h"			// predefined strings
#include "ListUtils.h"			// useful functions for nested lists
#include "Stream.h"			// wrapper for secondo streams
#include "GenericTC.h"			//use of generic type constructors
#include "LogMsg.h"			//send error messages
#include "../../Tools/Flob/DbArray.h"	//use of DbArrays
#include "RelationAlgebra.h"		//use of tuples
#include <math.h>			//require for some operators
#include <stack>
#include <limits>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

extern NestedList *nl;
extern QueryProcessor *qp;
extern AlgebraManager *am;

using namespace std;


namespace Redes{
 
class Arco
{
	private:
	int source,destination;
	char relacion[20];
	public:
	Arco(){source=0;destination=0;strcpy(relacion,"null");}
	Arco(const char* _relacion,const int _source,const int _destination){source=_source;destination=_destination;strcpy(relacion,_relacion);}
	~Arco(){};
	 static const string BasicType(){ return "arco";}
	  
	static const bool checkType(const ListExpr list) {
		return listutils::isSymbol(list, BasicType());
	  }
	int getSource(){return source;}
	int getDestination(){return destination;}
	string getRelacion(){return relacion;}
	bool setArco(int _source, int _destination,string _relacion){
		source=_source;destination=_destination;strcpy(relacion,_relacion.c_str());
	}
};

class Nodo
{
	private:
		int ID;
		char nombre[20];
	public:
	Nodo(){ID=0;strcpy(nombre,"NULL");}
	Nodo(const int _ID, const char* _nombre){ID=_ID;strcpy(nombre,_nombre);}
	//Nodo(const int _ID, const string _nombre):ID(_ID), nombre(_nombre){};
	~Nodo(){};
	 static const string BasicType(){ return "nodo";}
	  // the checktype function for non-nested types looks always the same

	 static const bool checkType(const ListExpr list) {
		return listutils::isSymbol(list, BasicType());
	}
	int getID(){return ID;}
	string getNombre(){return nombre;}	
	bool setNodo(int _ID,string _nombre){ID=_ID; strcpy(nombre,_nombre.c_str());return true;}
};


//Definición de clase circulo 
class Grafo
{
	private:
		string identificador;
		int cantNodos;
		int cantArcos;
		std::vector<Nodo> Nodos;
		std::vector<Arco> arcos;
		void resizeVec( std::vector<std::vector<int> > &vec , const unsigned short ROWS , const unsigned short COLUMNS )
		{
		    vec.resize( ROWS );
		    for( std::vector<std::vector<int> >::iterator it = vec.begin(); it != vec.end(); ++it)
		    {
		        it->resize( COLUMNS );
		    }

		    
		}
	public:
	  //constructor doing nothing
	  Grafo() {};
	  
	  //constructor initializing the object
	  Grafo(const string _identificador, const int _cantNodos, const int _cantArcos):identificador(_identificador),cantNodos(_cantNodos), cantArcos(_cantArcos){}
	  	 
	  //metodo para clonar;
	  Grafo(Grafo* k){
	  	arcos= *k->GetArcos();
	  	Nodos = *k->GetNodos();
	  	//falta rellenar la matriz
	  }

	  Grafo(vector<Nodo> _nodos, vector<Arco> _arco){Nodos =vector<Nodo>(_nodos);arcos = vector<Arco>(_arco);}
	  //destructor
	  ~Grafo(){}

	  static const string BasicType(){ return "grafo";}
	  // the checktype function for non-nested types looks always the same

	  static const bool checkType(const ListExpr list) {
		return listutils::isSymbol(list, BasicType());
	  }

	  bool AddNodo(const int ID, const string name){
	  	char aux[20];
		strcpy(aux,name.c_str());
	  	Nodo nodito=Nodo(ID,aux);
	  	Nodos.push_back(nodito);
	  	return true;

	  }

	  bool AddArco(const int ID1,const int ID2, const string name){
		char aux[20];
		strcpy(aux,name.c_str());
		Arco arc= Arco(aux,ID1,ID2);
		arcos.push_back(arc);  	
	  	//Arcos[ID1][ID2]=name;
	  	return true;
	  }
	  //para la funcion out
	  vector<Nodo>* GetNodos(){
	  	return &Nodos;

	  }

	  vector<Arco>* GetArcos(){
	  	return &arcos;

	  }
	  int nodosMasArcos() const {return Nodos.size()+arcos.size();}
	  int CantNodos() const {return Nodos.size();}
	  int CantArcos() const {return arcos.size();}
	  Nodo nodoMasPopular(){
	  	std::map<int,int> var;
	  	//vector<int> var;
		for (int i = 0; i < Nodos.size(); ++i)
	  	{
	  		var[Nodos[0].getID()]=0;
	  	}
	  	for (int i = 0; i < arcos.size(); ++i)
	  	{
	  		var[arcos[0].getDestination()]=var[arcos[0].getDestination()]+1;

	  	}
	  	int pop=-1;
	  	int index=-1;
	  	for (int i = 0; i < Nodos.size(); ++i){

	  		if(var[i]>pop){
	  			pop=var[i];
	  			index=i;
	  		}	
	  	}
	  	if(index==-1){
	  		return Nodo();}
	  	else{
	  		return Nodos[index];
	  		}
	  	
	  }

	  string relacion(string a1, string a2){
	  	bool var;
	  	int id1=-1,id2=-1;
	  	string aux;
	  	stringstream h,h1;
	  	for(int i =  0; i< Nodos.size() ;i++){
	  		if(Nodos[i].getNombre().compare(a1)==0) id1=Nodos[i].getID();
	  		else if(Nodos[i].getNombre().compare(a2)==0) id2=Nodos[i].getID();

	  	}
	  	if(id1==-1 || id2==-1) return "No tienen relacion";
	  	for (int i = 0; i < arcos.size(); ++i)
  		{
  			if((arcos[i].getSource()==id1 && arcos[i].getDestination()==id2)){
  				return arcos[i].getRelacion();
  			
  			}else if((arcos[i].getSource()==id2 && arcos[i].getDestination()==id1)){
  				return arcos[i].getRelacion();
  			}


  		}
  	

	  	return "No existe relacion";
	}


	  Nodo common(Nodo a1, Nodo a2){
	  	int id1=-1,id2=-1;
	  	string err="error";
	  	vector<int> first,second;
	  	vector<Nodo>salida;
	  	vector<int> third;
	  	//stringstream h,h1;
	  	// for(int i = 0; i< Nodos.size() ;i++){
	  	// 	if(Nodos[i].getNombre().compare(a1.getNombre())==0) id1=Nodos[i].getID();
	  	// 	else if(Nodos[i].getNombre().compare(a2.getNombre())==0) id2=Nodos[i].getID();

	  	// }
	  	//if(id1==-1 || id2==-1) return Nodo(-1,err.c_str());
	  	for (int i = 0; i < arcos.size(); ++i)
  		{
  			if(arcos[i].getSource()==a1.getID()){
  				first.push_back(arcos[i].getDestination());
  			
  			}else if(arcos[i].getDestination()==a1.getID()){
  				first.push_back(arcos[i].getSource());
  			}else if((arcos[i].getSource()==a2.getID())){
  				second.push_back(arcos[i].getDestination());
  			}else if(arcos[i].getDestination()==a2.getID()){
  				second.push_back(arcos[i].getSource());
  			}
  			
  		}
  	

  		for(vector<int>::iterator it=first.begin(); it != first.end(); ++it){
  			for (vector<int>::iterator ot=second.begin(); ot != second.end(); ++ot)
  			{
  				if(*ot==*it) third.push_back(*ot);
  			}
  		}
  		
  			for (int j = 0; j < third.size(); ++j)
  			{
  				for(int i = 0; i< Nodos.size() ;i++){
  				if(Nodos[i].getID()==third[j]) salida.push_back(Nodos[i]);
  				}
  			}
	  		
	  		

	  	
  		if(salida.size()==0)return Nodo(-1,err.c_str());
  		else return salida[0];
	}


	int radio(){
		int   N = Nodos.size(); // Number of nodes in graph
		int   INF = 0xFFFF; // Infinity
		int         d[N][N]; // Distances between nodes
		int         e[N]; // Eccentricity of nodes
		
		int         rad = INF; // Radius of graph


		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				d[i][j]=INF;
			}
			e[i]=-1;
		}

		for(vector<Arco>::iterator it=arcos.begin();it!=arcos.end();++it){

			d[it->getSource()][it->getDestination()]=1;
			d[it->getDestination()][it->getSource()]=1;
		}

		// Floyd-Warshall's algorithm
		for (int k = 0; k < N; k++) {
		    for (int j = 0; j < N; j++) {
		        for (int i = 0; i < N; i++) {
		            d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
		        }
		    }
		}

		// Counting values of eccentricity
		for (int i = 0; i < N; i++) {
		    for (int j = 0; j < N; j++) {
		    	if(d[i][j]!=INF)
		        e[i] = max(e[i], d[i][j]);
		    }
		}

		for (int i = 0; i < N; i++) {
			if(e[i]!=-1)
		    rad = min(rad, e[i]);
		}

	
		return rad;

	}

	int diameter(){
				int   N = Nodos.size(); // Number of nodes in graph
		int   INF = 0xFFFF; // Infinity
		int         d[N][N]; // Distances between nodes
		int         e[N]; // Eccentricity of nodes
		set <int>   c; // Center of graph
		int         diam=-1; // Diamater of graph

		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				d[i][j]=INF;
			}
			e[i]=-1;
		}

		for(vector<Arco>::iterator it=arcos.begin();it!=arcos.end();++it){

			d[it->getSource()][it->getDestination()]=1;
			d[it->getDestination()][it->getSource()]=1;
		}

		// Floyd-Warshall's algorithm
		for (int k = 0; k < N; k++) {
		    for (int j = 0; j < N; j++) {
		        for (int i = 0; i < N; i++) {
		            d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
		        }
		    }
		}

		// Counting values of eccentricity
		for (int i = 0; i < N; i++) {
		    for (int j = 0; j < N; j++) {
		    	if(d[i][j]!=INF)
		        e[i] = max(e[i], d[i][j]);
		    }
		}

		for (int i = 0; i < N; i++) {
		   	if(e[i]!=-1)
		    diam = max(diam, e[i]);
		}

		

		return diam;
	}

	Nodo central(){
				int   N = Nodos.size(); // Number of nodes in graph
		int   INF = 0xFFFF; // Infinity
		int         d[N][N]; // Distances between nodes
		int         e[N]; // Eccentricity of nodes
		vector<int>  c; // Center of graph
		int         rad = INF; // Radius of graph
		int         diam=-1; // Diamater of graph

		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				d[i][j]=INF;
			}
			e[i]=-1;
		}

		for(vector<Arco>::iterator it=arcos.begin();it!=arcos.end();++it){

			d[it->getSource()][it->getDestination()]=1;
			d[it->getDestination()][it->getSource()]=1;
		}

		// Floyd-Warshall's algorithm
		for (int k = 0; k < N; k++) {
		    for (int j = 0; j < N; j++) {
		        for (int i = 0; i < N; i++) {
		            d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
		        }
		    }
		}

		// Counting values of eccentricity
		for (int i = 0; i < N; i++) {
		    for (int j = 0; j < N; j++) {
		    	if(d[i][j]!=INF)
		        e[i] = max(e[i], d[i][j]);
		    }
		}

		for (int i = 0; i < N; i++) {
			if(e[i]!=-1)
		    rad = min(rad, e[i]);
		}

		for (int i = 0; i < N; i++) {
		    if (e[i] == rad) {
		        c.push_back(i);
		    }
		}

		
		for(vector<Nodo>::iterator it=Nodos.begin();it!=Nodos.end();++it){
			if(it->getID()==c.front()) return *it;
		
		}

		return Nodo(-1,"No existe");
	}
	bool nodoPertenece(Nodo node){
		for (std::vector<Nodo>::iterator i = Nodos.begin(); i != Nodos.end(); ++i)
		{
			if(i->getID()==node.getID() && i->getNombre().compare(node.getNombre())==0) return true;
		}
		return false;
	}

	bool arcoPertenece(Arco arco){
		for (std::vector<Arco>::iterator i = arcos.begin(); i != arcos.end(); ++i)
		{
			if(i->getSource()==arco.getSource() && i->getDestination()==arco.getDestination() && i->getRelacion().compare(arco.getRelacion())==0) return true;
		}
		return false;

	}

	bool borrarNodo(Nodo n){
		return false;
	}

	bool agregarNodo(Nodo n){
		return false;
	}

	bool borrarArco(Nodo n){
		return false;
	}

	bool agregarArco(Nodo n){
		return false;
	}	
};


// Property Function
// Esta función provee una descripción del tipo al usuario. Retorna una lista anidada
// que siempre tiene el mismo formato. El primer elemento de la lista es simpre el mismo
// y el segundo elemento de la lista contiene tipos específicos de descripciones.

ListExpr GrafoProperty() {
	return( nl->TwoElemList (
	  nl->FourElemList (
		nl->StringAtom("Signature"),
		nl->StringAtom("Example Type List"),
		nl->StringAtom("List Rep"),
		nl->StringAtom("Example List")),
	 nl->FourElemList (
		nl->StringAtom("-> SIMPLE"),
		nl->StringAtom(Grafo::BasicType()),
		nl->StringAtom(""),
		nl->StringAtom("(((1 Juan)) ((2 3 familiar)))")
	)));
}

ListExpr NodoProperty() {
	return( nl->TwoElemList (
	  nl->FourElemList (
		nl->StringAtom("Signature"),
		nl->StringAtom("Example Type List"),
		nl->StringAtom("List Rep"),
		nl->StringAtom("Example List")),
	 nl->FourElemList (
		nl->StringAtom("-> SIMPLE"),
		nl->StringAtom(Nodo::BasicType()),
		nl->StringAtom("(int string)=(ID nombre)"),
		nl->StringAtom("(1 Juan)")
	)));
}

ListExpr ArcoProperty() {
	return( nl->TwoElemList (
	  nl->FourElemList (
		nl->StringAtom("Signature"),
		nl->StringAtom("Example Type List"),
		nl->StringAtom("List Rep"),
		nl->StringAtom("Example List")),
	 nl->FourElemList (
		nl->StringAtom("-> SIMPLE"),
		nl->StringAtom(Nodo::BasicType()),
		nl->StringAtom("(int int string)=(source destination relacion)"),
		nl->StringAtom("(1 Juan)")
	)));
}


// In Function
// La tarea de la "In Function" es convertir una lista en una representación interna del
// del objeto, o sea tomar los datos que vienen en el formato de una lista y convertirlos
// en el formato de la clase (SCircle). La lista puede tener un formato inválido, en cuyo
// caso el parámetro de salida "correct" debe ser fijado en falso y el puntero addr debe
// fijado a 0. Una descripción detallada del error puede ser proveida al usuario llamando
// a inFunError del objeto global cmsg. En caso de éxito, el parámetro "correct" debe ser
// fijado como verdadero y el puntero addr apuntando a un objeto que tenga el valor
// representado por la instancia.


// typeInfo: contiene la descripción completa del tipo y es requerida para tipos anidados como tuplas
// instance: el valor del objeto en representación externa (nested list).
// errorPos: parámetro de salida reportando la posición de un error en la lista
// errorInfo: puede proveer información sobre el error al usuario
// correct: parametro de salida retornando el resultado de la llamada

Word InGrafo( const ListExpr typeInfo, const ListExpr instance, const int errorPos, ListExpr& errorInfo, bool& correct){
	
	Grafo* grafo ;
	grafo = new Grafo();
	//crea un resultado con addr apuntando a 0
	Word res((void*)0);

	//asume una lista incorrecta
	correct = false;

	//verifica si la lista contiene tres elementos

	if(!nl->HasLength(instance,2)){
	  cmsg.inFunError("expected two list");
	  return res;
	}

	if (nl->ListLength(instance) == 2) {


     ListExpr nodosList = nl->First(instance);
     ListExpr arcosList = nl->Second(instance);
     // si las listas no son elementos atomicos entonces:
     if (!(nl->IsAtom(nodosList) || nl->IsAtom(arcosList))) {

       correct = true;

       ListExpr first, second, third;
       ListExpr firstElem = nl->Empty();
       ListExpr rest = nodosList;

        // parse values of vertices
       while (correct && !nl->IsEmpty(rest)) {
         firstElem = nl->First(rest);
         rest = nl->Rest(rest);

         if (nl->ListLength(firstElem) != 2)
           correct = false;
         else {
           first = nl->First(firstElem);
           second = nl->Second(firstElem);
           //third = nl->Third(firstElem);
           
           if (!(nl->IsAtom(first) && (nl->AtomType(first) == IntType)
                && nl->IsAtom(second) && (nl->AtomType(second) == StringType)))
             correct = false;
           else
             correct = grafo->AddNodo(nl->IntValue(first),
                                      nl->StringValue(second));


           //nodo=(Nodo*)InNodo(nl->TheEmptyList(),first,0,errorInfo,correct).addr;	
           //if (correct) {
             //  correct = grafo->AddNodo(*nodo);
              // delete nodo;
             //}
           }
        }

     // parse values of edges
       firstElem = nl->Empty();
       rest = arcosList;

       while (correct && !nl->IsEmpty(rest)) {
         firstElem = nl->First(rest);
         rest = nl->Rest(rest);

        if (nl->ListLength(firstElem) != 3)
           correct = false;
        else {
           first = nl->First(firstElem);
           second = nl->Second(firstElem);
           third = nl->Third(firstElem);
           
           if (!(nl->IsAtom(first) && (nl->AtomType(first) == IntType)
                && nl->IsAtom(second) && (nl->AtomType(second) == IntType)
                && nl->IsAtom(third) && (nl->AtomType(third) == StringType)))
             correct = false;
           else
             correct = grafo->AddArco(nl->IntValue(first),
                                      nl->IntValue(second),
                                      nl->StringValue(third));
        }
       }



       }
   }


// 
	// //obtiene el valor numérico de los elementos
	// double cantNodos = listutils::getNumValue(nl->First(instance));
	// double cantArcos = listutils::getNumValue(nl->Second(instance));


	// //si la lista fue correcta, creamos el resultado
	// correct = true;
	// res.addr = new Grafo(cantNodos,cantArcos);
	// return res;


   if (correct)
     return SetWord(grafo);

   delete grafo;
   return SetWord(Address(0));
}

Word InNodo( const ListExpr typeInfo, const ListExpr instance, const int errorPos, ListExpr& errorInfo, bool& correct){
	
	Nodo* nodo ;
	nodo = new Nodo();
	//crea un resultado con addr apuntando a 0
	Word res((void*)0);

	//asume una lista incorrecta
	correct = false;

	//verifica si la lista contiene tres elementos

	if(!nl->HasLength(instance,2)){
	  cmsg.inFunError("expected dos elementos");
	  return res;
	}

	if (nl->ListLength(instance) == 2) {


	     ListExpr ID = nl->First(instance);
	     ListExpr nombre = nl->Second(instance);
	     // si las listas  son elementos atomicos entonces:
	     if ((nl->IsAtom(ID) && nl->IsAtom(nombre))) {

	       correct = true;

		    if (!(nl->IsAtom(ID) && (nl->AtomType(ID) == IntType)
		    	&& nl->IsAtom(nombre) && (nl->AtomType(nombre) == StringType)))
		    	correct = false;
		    else
		        correct = nodo->setNodo(nl->IntValue(ID),
		        						nl->StringValue(nombre));


	          
    	}
    }

   if (correct)
     return SetWord(nodo);

   delete nodo;
   return SetWord(Address(0));
}

Word InArco( const ListExpr typeInfo, const ListExpr instance, const int errorPos, ListExpr& errorInfo, bool& correct){
	
	Arco* arco ;
	arco = new Arco();
	//crea un resultado con addr apuntando a 0
	Word res((void*)0);

	//asume una lista incorrecta
	correct = false;

	//verifica si la lista contiene tres elementos

	if(!nl->HasLength(instance,3)){
	  cmsg.inFunError("expected tres elementos");
	  return res;
	}

	if (nl->ListLength(instance) == 3) {


	     ListExpr source = nl->First(instance);
	     ListExpr destination = nl->Second(instance);
	     ListExpr name = nl->Third(instance);
	     // si las listas  son elementos atomicos entonces:
	     if ((nl->IsAtom(source) && nl->IsAtom(destination) && nl->IsAtom(name))) {

	       correct = true;

		    if (!(nl->IsAtom(source) && (nl->AtomType(source) == IntType)
		    	&& nl->IsAtom(destination) && (nl->AtomType(destination) == IntType)
		    	&& nl->IsAtom(name) && (nl->AtomType(name) == StringType)))
		    	correct = false;
		    else
		        correct = arco->setArco(nl->IntValue(source),
		        						nl->IntValue(destination),
		        						nl->StringValue(name));


	          
    	}
    }

   if (correct)
     return SetWord(arco);

   delete arco;
   return SetWord(Address(0));
}



// Out Function
// Esta funcion se utiliza convertir cada instancia de la clase en una lista anidada.
// Por tal motivo no función para reportar error como sucede en las In Function.

ListExpr OutGrafo( ListExpr typeInfo, Word value){
	Grafo* grafo = (Grafo*) value.addr;
	vector<Nodo>* n = grafo->GetNodos();
	ListExpr last;
    ListExpr verticesList;
// crear ListExpr para vertices
    if(n->size()>0){
	    verticesList= nl->OneElemList(
	    	nl->TwoElemList(
	    	 	nl->IntAtom((*n)[0].getID()),
	            nl->StringAtom((*n)[0].getNombre())
	            )
	    );
		last= verticesList;
		for(int i=1;i<(*n).size();i++){
			last=
			nl->Append(
				last,
				nl->TwoElemList(
					nl->IntAtom((*n)[i].getID()),
					nl->StringAtom((*n)[i].getNombre())
				)
			);

		}
	}
	else{

		verticesList = nl->TheEmptyList();

	}
	//delete n;
	

	vector<Arco>* m = grafo->GetArcos();
	ListExpr arcosList;

	if(m->size()>0){
	    arcosList= nl->OneElemList(
	    	nl->ThreeElemList(
	    	 	nl->IntAtom((*m)[0].getSource()),
	    	 	nl->IntAtom((*m)[0].getDestination()),
	            nl->StringAtom((*m)[0].getRelacion())
	            )
	    );
		last= arcosList;
		for(int i=1;i<(*m).size();i++){
			last=
			nl->Append(
				last,
				nl->ThreeElemList(
	    	 	nl->IntAtom((*m)[i].getSource()),
	    	 	nl->IntAtom((*m)[i].getDestination()),
	            nl->StringAtom((*m)[i].getRelacion())
	            )
			);

		}
	}
	else{

		arcosList = nl->TheEmptyList();

	}
	//delete m;
	
	return (nl->TwoElemList(verticesList,arcosList));



}

ListExpr OutNodo( ListExpr typeInfo, Word value){
	Nodo* nodo = (Nodo*) value.addr;
	ListExpr aux;
    //ListExpr verticesList;
// crear ListExpr para vertices
    aux= nl->OneElemList(
	    	nl->TwoElemList(
	    	 	nl->IntAtom((*nodo).getID()),
	            nl->StringAtom((*nodo).getNombre())
	            )
	    );
	return (nl->OneElemList(aux));



}

ListExpr OutArco( ListExpr typeInfo, Word value){
	Arco* arco = (Arco*) value.addr;
	ListExpr aux;
    //ListExpr verticesList;
// crear ListExpr para vertices
    aux= nl->OneElemList(
	    	nl->ThreeElemList(
	    	 	nl->IntAtom((*arco).getSource()),
	    	 	nl->IntAtom((*arco).getDestination()),
	            nl->StringAtom((*arco).getRelacion())
	            )
	    );
	return (nl->OneElemList(aux));



}

// Create Function
// Esta función crea un objeto con valor arbitrario

Word CreateGrafo( const ListExpr typeInfo){
	Word w;
	w.addr = (new Grafo());
	return w;
}

Word CreateNodo( const ListExpr typeInfo){
	Word w;
	w.addr = (new Nodo());
	return w;
}

Word CreateArco( const ListExpr typeInfo){
	Word w;
	w.addr = (new Arco());
	return w;
}
// Delete Function
// Remueve completamente el objeto (incluyendo las partes en disco si es que hay)

void DeleteGrafo( const ListExpr typeInfo, Word& w){
	Grafo *k = (Grafo *)w.addr;
	delete k;
	w.addr = 0;
}

void DeleteNodo( const ListExpr typeInfo, Word& w){
	Nodo *k = (Nodo *)w.addr;
	delete k;
	w.addr = 0;
}

void DeleteArco( const ListExpr typeInfo, Word& w){
	Arco *k = (Arco *)w.addr;
	delete k;
	w.addr = 0;
}
// Open Function
// Lee un objeto desde disco via SmiRecord
// valueRecord: the disc representation of the object is stored
// offset: la representación del objeto comienza en el valueRecord despues de la llamada
//         de esta función, el offset debe ser despúes del valor del objeto
// typeInfo: la descripción del tipo (requerida para tipos complejos)
// value: argumento de salida

bool OpenGrafo(SmiRecord& valueRecord, size_t& offset, const ListExpr typeInfo, Word& value){

	int nodo_size,arco_size;

  	size_t integer=sizeof(int);
 	size_t varchar = sizeof(char)*20;
 	vector<Nodo> nodo;
 	vector<Arco> arco;
 	
 	bool ok=(valueRecord.Read(&nodo_size,integer,offset)==integer);
 	offset+= integer;
 	ok= ok&& (valueRecord.Read(&arco_size,integer,offset)==integer);
 	offset +=integer;

	//int id1,id2,id;
 	//string name,rel;
 
 	for(int j=0;j<nodo_size;j++){
 		int id ;
 		char aux[20];
 		ok= ok&& (valueRecord.Read(&id,integer,offset)==integer);
 		offset +=integer;
 		ok= ok&& (valueRecord.Read(aux,varchar,offset)==varchar);
 		offset +=varchar;
 		string str(aux);
 		Nodo node=Nodo(id,aux);
 		nodo.push_back(node);
 	}


 	for(int j=0;j<arco_size;j++){
 		int id1 ;
 		int id2;
 		char aux[20];
 		ok= ok&& (valueRecord.Read(&id1,integer,offset)==integer);
 		offset +=integer;
 		ok= ok&& (valueRecord.Read(&id2,integer,offset)==integer);
 		offset +=integer;
 		ok= ok&& (valueRecord.Read(aux,varchar,offset)==varchar);
 		offset +=varchar;
 		string str(aux);
 		Arco arc=Arco(aux,id1,id2);
 		arco.push_back(arc);
 	}




 	if(ok){
 		value.addr = new Grafo(nodo,arco);

 	}else {
 		value.addr = 0;
 	}
 		return 0;

}

bool OpenNodo(SmiRecord& valueRecord, size_t& offset, const ListExpr typeInfo, Word& value){
	size_t integer=sizeof(int);
 	size_t varchar = sizeof(char)*20;
 	int ID;
 	
 	char aux[20];
// size_t size = sizeof(double);
	// double cantNodos,cantArcos;
	 bool ok = (valueRecord.Read(&ID,integer,offset) == integer);
	  offset+= integer;
	 ok = ok&& (valueRecord.Read(aux,varchar,offset) == varchar);
	  offset+= varchar;
	 if(ok){
	   value.addr = new Nodo(ID,aux);
	 }
	else {
	  value.addr = 0;
	}
	return ok;

}

bool OpenArco(SmiRecord& valueRecord, size_t& offset, const ListExpr typeInfo, Word& value){
	size_t integer=sizeof(int);
 	size_t varchar = sizeof(char)*20;
 	int source,destination;
 	
 	char aux[20];
// size_t size = sizeof(double);
	// double cantNodos,cantArcos;
	 bool ok = (valueRecord.Read(&source,integer,offset) == integer);
	  offset+= integer;
	  ok = (valueRecord.Read(&destination,integer,offset) == integer);
	  offset+= integer;
	 ok = ok&& (valueRecord.Read(aux,varchar,offset) == varchar);
	  offset+= varchar;
	 if(ok){
	   value.addr = new Arco(aux,source,destination);
	 }
	else {
	  value.addr = 0;
	}
	return ok;

}


// Save Function
bool SaveGrafo( SmiRecord& valueRecord, size_t& offset, const ListExpr typeInfo, Word&value){
	
	Grafo* k = static_cast<Grafo*>(value.addr);
	size_t size_int = sizeof(int);
	size_t size_string = sizeof(char)*20;
	bool ok=true;
	vector<Nodo>* nodo = k->GetNodos();
	int nodo_size=nodo->size();
	vector<Arco>* arco = k->GetArcos();
	int arco_size=arco->size();
	//ingresaremos los largos de cada vector al disco para luego saber hasta donde llegamos
	ok = ok && valueRecord.Write(&nodo_size, size_int, offset);
	offset += size_int;
	ok = ok && valueRecord.Write(&arco_size, size_int, offset);
	offset += size_int;
	


	
	for(vector<Nodo>::iterator it=nodo->begin(); it != nodo->end(); ++it){
		int aux1=it->getID();
		ok = ok && valueRecord.Write(&aux1, size_int, offset);
		offset += size_int;
		char aux[20];
		string aux2=it->getNombre();
		strcpy(aux,aux2.c_str());
		ok = ok && valueRecord.Write(aux, size_string, offset);
		offset += size_string;
	}


	
	for(vector<Arco>::iterator it=arco->begin(); it != arco->end(); ++it){
		int aux1=it->getSource();
		ok = ok && valueRecord.Write(&aux1, size_int, offset);
		offset += size_int;
		int aux2=it->getDestination();
		ok = ok && valueRecord.Write(&aux2, size_int, offset);
		offset += size_int;		
		char aux[20];
		string aux3=it->getRelacion();
		strcpy(aux,aux3.c_str());
		ok = ok && valueRecord.Write(aux, size_string, offset);
		offset += size_string;
	}
	
	return ok;
}


bool SaveNodo( SmiRecord& valueRecord, size_t& offset, const ListExpr typeInfo, Word&value){
	
	Nodo* k = static_cast<Nodo*>(value.addr);
	size_t size_int = sizeof(int);
	size_t size_string = sizeof(char)*20;
	bool ok=true;
	int ID = k->getID();
	char aux[20];
	strcpy(aux,k->getNombre().c_str());
	//ingresaremos los largos de cada vector al disco para luego saber hasta donde llegamos
	ok = ok && valueRecord.Write(&ID, size_int, offset);
	offset += size_int;
	ok = ok && valueRecord.Write(aux, size_string, offset);
	offset += size_string;
	


	
	return ok;
}

bool SaveArco( SmiRecord& valueRecord, size_t& offset, const ListExpr typeInfo, Word&value){
	
	Arco* k = static_cast<Arco*>(value.addr);
	size_t size_int = sizeof(int);
	size_t size_string = sizeof(char)*20;
	bool ok=true;
	int source = k->getSource();
	int destination= k->getDestination();
	char aux[20];
	strcpy(aux,k->getRelacion().c_str());
	//ingresaremos los largos de cada vector al disco para luego saber hasta donde llegamos
	ok = ok && valueRecord.Write(&source, size_int, offset);
	offset += size_int;
	ok = ok && valueRecord.Write(&destination, size_int, offset);
	offset += size_int;
	ok = ok && valueRecord.Write(aux, size_string, offset);
	offset += size_string;
	


	
	return ok;
}

// Close Function
// Remueve el objeto desde memoria principal, la parte en disco 
// (si es que existe) queda sin cambios.

void CloseGrafo( const ListExpr typeInfo, Word& w){
	Grafo * k = (Grafo *)w.addr;
	delete k;
	w.addr = 0;

}

void CloseNodo( const ListExpr typeInfo, Word& w){
	Nodo * k = (Nodo *)w.addr;
	delete k;
	w.addr = 0;

}

void CloseArco( const ListExpr typeInfo, Word& w){
	Arco * k = (Arco *)w.addr;
	delete k;
	w.addr = 0;

}
// Clone Function

Word CloneGrafo( const ListExpr typeInfo, const Word& w){
	Grafo * k = (Grafo*) w.addr;
	Word res;
	res.addr = new Grafo(k);
	return res;
}

Word CloneNodo( const ListExpr typeInfo, const Word& w){
	Nodo * k = (Nodo*) w.addr;
	Word res;
	res.addr = new Nodo(k->getID(),k->getNombre().c_str());
	return res;
}

Word CloneArco( const ListExpr typeInfo, const Word& w){
	Arco * k = (Arco*) w.addr;
	Word res;
	res.addr = new Arco(k->getRelacion().c_str(), k->getSource(),k->getDestination());
	return res;
}
// Cast Function
// Realiza un Cast a un puntero vacio al tipo especificado

void* CastGrafo( void* addr){
	return (new (addr) Grafo);
}

void* CastNodo( void* addr){
	return (new (addr) Nodo);
}

void* CastArco( void* addr){
	return (new (addr) Arco);
}
// Type Check Function

bool GrafoTypeCheck(ListExpr type, ListExpr& errorInfo){
	return nl->IsEqual(type, Grafo::BasicType());
}

bool NodoTypeCheck(ListExpr type, ListExpr& errorInfo){
	return nl->IsEqual(type, Nodo::BasicType());
}

bool ArcoTypeCheck(ListExpr type, ListExpr& errorInfo){
	return nl->IsEqual(type, Arco::BasicType());
}
// Size of Function

int SizeOfGrafo(){
	return sizeof(Grafo);
}
int SizeOfNodo(){
	return sizeof(Nodo);
}
int SizeOfArco(){
	return sizeof(Arco);
}

//TypeConstructor Instance

TypeConstructor GrafoTC(
	Grafo::BasicType(), 		//nombre del tipo
	GrafoProperty,			//property function
	OutGrafo, InGrafo,		// In & Out Function
	0,0,				// por que sí (antigua funcionalidad eliminada)
	CreateGrafo, DeleteGrafo,	// creacion y borrado
	OpenGrafo, SaveGrafo,		// apertura y guardado
	CloseGrafo, CloneGrafo,		// cierre y clonado
	CastGrafo,	
	SizeOfGrafo,			// funcion SizeOf
	GrafoTypeCheck);		// funcion de verificacion de tipo
	
TypeConstructor NodoTC(
	Nodo::BasicType(), 		//nombre del tipo
	NodoProperty,			//property function
	OutNodo, InNodo,		// In & Out Function
	0,0,				// por que sí (antigua funcionalidad eliminada)
	CreateNodo, DeleteNodo,	// creacion y borrado
	OpenNodo, SaveNodo,		// apertura y guardado
	CloseNodo, CloneNodo,		// cierre y clonado
	CastNodo,	
	SizeOfNodo,			// funcion SizeOf
	NodoTypeCheck);		// funcion de verificacion de tipo

TypeConstructor ArcoTC(
	Arco::BasicType(), 		//nombre del tipo
	ArcoProperty,			//property function
	OutArco, InArco,		// In & Out Function
	0,0,				// por que sí (antigua funcionalidad eliminada)
	CreateArco, DeleteArco,	// creacion y borrado
	OpenArco, SaveArco,		// apertura y guardado
	CloseArco, CloneArco,		// cierre y clonado
	CastArco,	
	SizeOfArco,			// funcion SizeOf
	ArcoTypeCheck);		// funcion de verificacion de tipo



// Imprementacion de operador

// Type Mapping

ListExpr nodosMasArcosTM(ListExpr args){
	string err = "grafo expected";
	// verifica el número de argumentos
	if(!nl->HasLength(args,1)){
	  return listutils::typeError(err);	
	}
	if(!Grafo::checkType(nl->First(args))){return listutils::typeError(err);}
	// retorna el tipo del resultado
	return listutils::basicSymbol<CcInt>();
}
ListExpr radioTM(ListExpr args){
	string err = "grafo expected";
	// verifica el número de argumentos
	if(!nl->HasLength(args,1)){
	  return listutils::typeError(err);	
	}
	if(!Grafo::checkType(nl->First(args))){return listutils::typeError(err);}
	// retorna el tipo del resultado
	return listutils::basicSymbol<CcInt>();
}

ListExpr diameterTM(ListExpr args){
	string err = "grafo expected";
	// verifica el número de argumentos
	if(!nl->HasLength(args,1)){
	  return listutils::typeError(err);	
	}
	if(!Grafo::checkType(nl->First(args))){return listutils::typeError(err);}
	// retorna el tipo del resultado
	return listutils::basicSymbol<CcInt>();
}

ListExpr CantNodosTM(ListExpr args){
	string err = "grafo expected";
	// verifica el número de argumentos
	if(!nl->HasLength(args,1)){
	  return listutils::typeError(err);	
	}
	if(!Grafo::checkType(nl->First(args))){return listutils::typeError(err);}
	// retorna el tipo del resultado
	return listutils::basicSymbol<CcInt>();
}
ListExpr CantArcosTM(ListExpr args){
	string err = "grafo expected";
	// verifica el número de argumentos
	if(!nl->HasLength(args,1)){
	  return listutils::typeError(err);	
	}
	if(!Grafo::checkType(nl->First(args))){return listutils::typeError(err);}
	// retorna el tipo del resultado
	return listutils::basicSymbol<CcInt>();
}
//FALTA CAMBIARLO POR NODOS
ListExpr relacionTM(ListExpr args){
	string err = "grafo expected";
	if(!nl->HasLength(args,3)){
		return listutils::typeError("TAMAÑO");

	}else {
		if(!Grafo::checkType(nl->First(args))){return listutils::typeError("TIPO GRAFO");}
		ListExpr first=nl->Second(args);
		ListExpr second = nl->Third(args);
		if(!CcString::checkType(first) ) return listutils::typeError("ingrese string");
		if(!CcString::checkType(second) ) return listutils::typeError("ingrese string");
	}
	return listutils::basicSymbol<CcString>();
}

ListExpr nodoMasPopularTM(ListExpr args){
	string err = "grafo expected";
	if(!nl->HasLength(args,1)){
		return listutils::typeError(err);

	}
	if(!Grafo::checkType(nl->First(args))){return listutils::typeError(err);}
	return nl->SymbolAtom(Nodo::BasicType());
}

ListExpr centralTM(ListExpr args){
	string err = "grafo expected";
	if(!nl->HasLength(args,1)){
		return listutils::typeError(err);

	}
	if(!Grafo::checkType(nl->First(args))){return listutils::typeError(err);}
	return nl->SymbolAtom(Nodo::BasicType());
}

ListExpr commonTM(ListExpr args){
	string err = "grafo expected";
	if(!nl->HasLength(args,3)){
		return listutils::typeError("TAMAÑO");

	}else {
		if(!Grafo::checkType(nl->First(args))){return listutils::typeError("TIPO GRAFO");}
		if(!Nodo::checkType(nl->Second(args))){return listutils::typeError("TIPO Nodo");}
		if(!Nodo::checkType(nl->Third(args))){return listutils::typeError("TIPO Nodo");}
		}
	return nl->SymbolAtom(Nodo::BasicType());
}

ListExpr nodoPerteneceTM(ListExpr args){
	string err = "grafo expected";
	if(!nl->HasLength(args,2)){
		return listutils::typeError("TAMAÑO");

	}else {
		if(!Grafo::checkType(nl->First(args))){return listutils::typeError("TIPO GRAFO");}
		if(!Nodo::checkType(nl->Second(args))){return listutils::typeError("TIPO Nodo");}
		}
	return listutils::basicSymbol<CcBool>();
}

ListExpr arcoPerteneceTM(ListExpr args){
	string err = "grafo expected";
	if(!nl->HasLength(args,2)){
		return listutils::typeError("TAMAÑO");

	}else {
		if(!Grafo::checkType(nl->First(args))){return listutils::typeError("TIPO GRAFO");}
		if(!Arco::checkType(nl->Second(args))){return listutils::typeError("TIPO Arco");}
		}
	return listutils::basicSymbol<CcBool>();
}
// Value Mapping



int nodosMasArcosVM(Word* args, Word& result, int message, Word& local, Supplier s){
	Grafo* k = (Grafo*) args[0].addr; // obtiene el argumento y hace un cast
	result = qp->ResultStorage(s);
	CcInt* res = (CcInt*) result.addr;
	res->Set(true, k->nodosMasArcos());
	return 0;
}

int radioVM(Word* args, Word& result, int message, Word& local, Supplier s){
	Grafo* k = (Grafo*) args[0].addr; // obtiene el argumento y hace un cast
	result = qp->ResultStorage(s);
	CcInt* res = (CcInt*) result.addr;
	res->Set(true, k->radio());
	return 0;
}

int diameterVM(Word* args, Word& result, int message, Word& local, Supplier s){
	Grafo* k = (Grafo*) args[0].addr; // obtiene el argumento y hace un cast
	result = qp->ResultStorage(s);
	CcInt* res = (CcInt*) result.addr;
	res->Set(true, k->diameter());
	return 0;
}

int CantNodosVM(Word* args, Word& result, int message, Word& local, Supplier s){
	Grafo* k = (Grafo*) args[0].addr; // obtiene el argumento y hace un cast
	result = qp->ResultStorage(s);
	CcInt* res = (CcInt*) result.addr;
	res->Set(true, k->CantNodos());
	return 0;
}
int CantArcosVM(Word* args, Word& result, int message, Word& local, Supplier s){
	Grafo* k = (Grafo*) args[0].addr; // obtiene el argumento y hace un cast
	result = qp->ResultStorage(s);
	CcInt* res = (CcInt*) result.addr;
	res->Set(true, k->CantArcos());
	return 0;
}

int nodoMasPopularVM(Word* args, Word& result, int message, Word& local, Supplier s){
	Grafo* k = (Grafo*) args[0].addr; // obtiene el argumento y hace un cast
	result = qp->ResultStorage(s);
	Nodo* res = (Nodo*) result.addr;
	*res=k->nodoMasPopular();
	return 0;
}

int centralVM(Word* args, Word& result, int message, Word& local, Supplier s){
	Grafo* k = (Grafo*) args[0].addr; // obtiene el argumento y hace un cast
	result = qp->ResultStorage(s);
	Nodo* res = (Nodo*) result.addr;
	*res=k->central();
	return 0;
}

int relacionVM(Word* args, Word& result, int message, Word& local, Supplier s){
	Grafo* k = (Grafo*) args[0].addr; // obtiene el argumento y hace un cast
	if(!((CcString*)args[1].addr)->IsDefined()){
		 return 0;
	}
	string a1(((CcString*)args[1].addr)->GetValue());

	if(!((CcString*)args[2].addr)->IsDefined()){
		return 0;
	}
	string a2(((CcString*)args[2].addr)->GetValue());
	result = qp->ResultStorage(s);
	CcString* res = (CcString*) result.addr;
	res->Set(true, k->relacion(a1,a2));
	return 0;
}

int commonVM(Word* args, Word& result, int message, Word& local, Supplier s){
	Grafo* k = (Grafo*) args[0].addr; // obtiene el argumento y hace un cast
	Nodo* a1= (Nodo*) args[1].addr;
	Nodo* a2= (Nodo*) args[2].addr;
	result = qp->ResultStorage(s);
	Nodo* res = (Nodo*) result.addr;
	*res=k->common(*a1,*a2);
	return 0;
}

int nodoPerteneceVM(Word* args, Word& result, int message, Word& local, Supplier s){
	Grafo* k = (Grafo*) args[0].addr; // obtiene el argumento y hace un cast
	Nodo* a1= (Nodo*) args[1].addr;
	result = qp->ResultStorage(s);
	CcBool* res = (CcBool*) result.addr;
	res->Set(true,k->nodoPertenece(*a1));
	return 0;
}

int arcoPerteneceVM(Word* args, Word& result, int message, Word& local, Supplier s){
	Grafo* k = (Grafo*) args[0].addr; // obtiene el argumento y hace un cast
	Arco* a1= (Arco*) args[1].addr;
	result = qp->ResultStorage(s);
	CcBool* res = (CcBool*) result.addr;
	res->Set(true,k->arcoPertenece(*a1));
	return 0;
}
// Especificación
OperatorSpec nodosMasArcosSpec(
	"grafo-> int",	
	"nodosMasArcos(_)",
	"Suma el numero de nodos y arcos.",
	"query nodosMasArcos([const grafo value (8 10)])"
);

OperatorSpec radioSpec(
	"grafo-> int",	
	"radio(_)",
	"radio del grafo.",
	"query radio([const grafo value (8 10)])"
);

OperatorSpec diameterSpec(
	"grafo-> int",	
	"diameter(_)",
	"diametro del grafo.",
	"query diameter([const grafo value (8 10)])"
);


OperatorSpec CantNodosSpec(
	"grafo-> int",	
	"CantNodos(_)",
	"Devuelve la cantidad de Nodos",
	"query CantNodos([const grafo value (8 10)])"
);
OperatorSpec CantArcosSpec(
	"grafo-> int",	
	"CantArcos(_)",
	"Devuelve la cantidad de Arcos",
	"query CantArcos([const grafo value (8 10)])"
);
OperatorSpec nodoMasPopularSpec(
	"grafo-> nodo",	
	"nodoMasPopular(_)",
	"Encuentra el nodo con mayor conexiones.",
	"query nodosMasArcos(var_tipo_grafo)"
);

OperatorSpec centralSpec(
	"grafo-> nodo",	
	"central(_)",
	"Encuentra el nodo central del grafo.",
	"query central(var_tipo_grafo)"
);

OperatorSpec relacionSpec(
	"grafo x string x string -> string",	
	"relacion(_,_,_)",
	"retorna el nombre de relacion entre dos nodos.",
	"query relacion(var_tipo_grafo,string1,string2)"
);
OperatorSpec commonSpec(
	"grafo x nodo x nodo -> nodo",	
	"common(_,_,_)",
	"retorna el primero nodo comun encontrado entre dos nodos.",
	"query common(var_tipo_grafo,nodo1,nodo2)"
);
OperatorSpec nodoPerteneceSpec(
	"grafo x nodo  -> bool",	
	"nodoPertenece(_,_)",
	"verdadero si el nodo pertenece al grafo, falso sino.",
	"query nodoPertenece(var_tipo_grafo,nodo1)"
);

OperatorSpec arcoPerteneceSpec(
	"grafo x arco  -> bool",	
	"arcoPertenece(_,_)",
	"verdadero si el arco pertenece al grafo, falso sino.",
	"query arcoPertenece(var_tipo_grafo,arco1)"
);
//Instanciacion del Operador
Operator nodosMasArcosOp(
	"nodosMasArcos",
	nodosMasArcosSpec.getStr(),
	nodosMasArcosVM,
	Operator::SimpleSelect,
	nodosMasArcosTM);
Operator radioOp(
	"radio",
	radioSpec.getStr(),
	radioVM,
	Operator::SimpleSelect,
	radioTM);

Operator diameterOp(
	"diameter",
	diameterSpec.getStr(),
	diameterVM,
	Operator::SimpleSelect,
	diameterTM);

Operator CantNodosOp(
	"CantNodos",
	CantNodosSpec.getStr(),
	CantNodosVM,
	Operator::SimpleSelect,
	CantNodosTM);

Operator CantArcosOp(
	"CantArcos",
	CantArcosSpec.getStr(),
	CantArcosVM,
	Operator::SimpleSelect,
	CantArcosTM);


Operator nodoMasPopularOp(
	"nodoMasPopular",
	nodoMasPopularSpec.getStr(),
	nodoMasPopularVM,
	Operator::SimpleSelect,
	nodoMasPopularTM);	

Operator centralOp(
	"central",
	centralSpec.getStr(),
	centralVM,
	Operator::SimpleSelect,
	centralTM);

Operator relacionOp(
	"relacion",
	relacionSpec.getStr(),
	relacionVM,
	Operator::SimpleSelect,
	relacionTM);
Operator commonOp(
	"common",
	commonSpec.getStr(),
	commonVM,
	Operator::SimpleSelect,
	commonTM);

Operator nodoPerteneceOp(
	"nodoPertenece",
	nodoPerteneceSpec.getStr(),
	nodoPerteneceVM,
	Operator::SimpleSelect,
	nodoPerteneceTM);

Operator arcoPerteneceOp(
	"arcoPertenece",
	arcoPerteneceSpec.getStr(),
	arcoPerteneceVM,
	Operator::SimpleSelect,
	arcoPerteneceTM);


class RedesAlgebra: public Algebra{
	public:
	  RedesAlgebra() : Algebra(){
	  	AddTypeConstructor(&GrafoTC);
	  	AddTypeConstructor(&NodoTC);
	  	AddTypeConstructor(&ArcoTC);
		GrafoTC.AssociateKind(Kind::SIMPLE());
		NodoTC.AssociateKind(Kind::SIMPLE());
		ArcoTC.AssociateKind(Kind::SIMPLE());
		AddOperator(&nodosMasArcosOp);
		AddOperator(&nodoMasPopularOp);	
	  	AddOperator(&relacionOp);
	  	AddOperator(&commonOp);
	  	AddOperator(&CantNodosOp);
	  	AddOperator(&CantArcosOp);
	  	AddOperator(&radioOp);
	  	AddOperator(&diameterOp);
	  	AddOperator(&centralOp);
	  	AddOperator(&nodoPerteneceOp);
	  	AddOperator(&arcoPerteneceOp);
	  }
	  ~RedesAlgebra() {};
};

}// fin del namespace

extern "C"
Algebra* InitializeRedesAlgebra( NestedList* nlRef, QueryProcessor* qpRef, AlgebraManager* amRef){

	nl = nlRef;
  	qp = qpRef;
	am = amRef;
	return (new Redes::RedesAlgebra);
}








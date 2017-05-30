/* m3ta3: reimagination of a (late 2011, early 2012) personal, primordial visualization library that inspired further developments at UVic, CFS and elsewhere.. */
#include "../../glut2/newzpr.h"
#include<fstream>
#include<iostream>

long int getFileSize(std::string  fn){
  ifstream i;
  i.open(fn.c_str(), ios::binary);
  if(!i.is_open()){
    cout << "error: couldn't open file: " << fn << endl;
    return (-1);
  }
  i.seekg(0,ios::end);
  long int len = i.tellg();
  return(len);
}

int main(int argc, char * argv[]){
  if(sizeof(float) != 4){
    printf("Error: sizeof(float) != 4\n"); 
    exit(1);
  }

	if(argc <5){
		printf("imv.cpp: [infile] [nrow] [ncol] [nb]\n"); exit(1); 
	}

	char * infile = argv[1];
	long int NRow = (long int)atoi(argv[2]);
	long int NCol = (long int)atoi(argv[3]);
	long int NBand = (long int)atoi(argv[4]);
	
  printf(" infile: %s nrow %ld ncol %ld nband %ld\n", infile, NRow, NCol, NBand);
	printf(" getFileSize %ld expected %ld\n", getFileSize(infile), NRow*NCol*NBand*sizeof(float));
  
  FILE * f = fopen(infile, "rb");
	SA<float> dat(NRow*NCol*NBand);
	long int nread = fread( &dat[0], sizeof(float), NRow*NCol*NBand, f);
	if((nread)!=(NRow*NCol*NBand)){
    printf("Error (imv.cpp): number of elements read (%d) different than expected (%d)\n", nread, NRow*NCol*NBand); 
    exit(1);
  }
	fclose(f);
	
  myImg a;
	a.initFrom(&dat, NRow, NCol, NBand);
	zprManager * myManager = zprManager::Instance(argc,argv);
	zprInstance * myZpr = myManager->newZprInstance(NRow, NCol,NBand);
	glImage * myImage = new glImage( myZpr, &a);
	glutMainLoop();
	return 0;
}


#include "../imv/util.h"
#include "../../SA.h"
int main(int argc, char ** argv){
	  if(argc < 4){
      // input file always BSQ, ENVI type 4, byte order 0
		  printf("multilook.cpp: [input file] [output file] [WINDOW SIZE]\n");
      exit(1); 
	  }

	  char * infile = argv[1];
    char * outfile = argv[2];
    int WINDOW_SIZE = atoi(argv[3]);
	  long int NRow, NCol, NBand; 
    parseHeaderFile(getHeaderFileName(string(infile)), NRow, NCol, NBand);

    printf(" infile: %s nrow %ld ncol %ld nband %ld\n", infile, NRow, NCol, NBand);
	  printf(" getFileSize %ld expected %ld\n", getFileSize(infile), NRow*NCol*NBand*sizeof(float));

    long int i, j, k, ci;
    float sampleN, d;

    /* output image dimensions */
    long int NR2 = NRow/WINDOW_SIZE; 
    long int NC2 = NCol/WINDOW_SIZE;

    /* output buffer */
    long int NS = NR2 * NC2 * NBand;// * sizeof(float);
    SA<float> dd2(NR2 * NC2 * NBand);
    SA<float> ddS(NR2 * NC2 * NBand);
    for(i=0; i < NS; i++){
      dd2.at(i) = ddS.at(i) = 0.;
    }


    /* input buffer */
    FILE * f = fopen(infile, "rb");
	  SA<float> dd(NRow * NCol * NBand);
	  long int nread = fread(&dd[0], sizeof(float), NRow * NCol * NBand, f);
	  if(nread != NRow * NCol * NBand){
      printf("Error (imv.cpp): number of elements read (%ld) different than expected (%ld)\n", nread, NRow*NCol*NBand); 
      exit(1);
    }
	  fclose(f);


    for(i=0; i<NRow; i++){
      for(j=0; j<NCol; j++){
        for(k=0; k<NBand; k++){
          ci = (k*NR2*NC2) + ((i/WINDOW_SIZE)*NC2) +(j/WINDOW_SIZE);
          if(ci >= NS) continue;
          float d = dd.at(k*NRow*NCol + i*NCol +j);
          if(!isinf(d) && !isnan(d)){
            dd2.at(ci) += d;
            ddS.at(ci) += 1.;
          }
        }
      }
    }

    for(i=0; i<NRow; i++){
      for(j=0; j<NCol; j++){
        for(k=0; k<NBand; k++){
          ci = (k*NR2*NC2) + ((i/WINDOW_SIZE)*NC2) +(j/WINDOW_SIZE);
          if(ci >= NS)continue;
          if(ddS.at(ci) > 0.){
            dd2.at(ci) /= ddS.at(ci);
          }
          else{
            dd2.at(ci) = 0.;
          }
        }
      }
    }

    f = fopen(outfile, "wb");
    if(!f){
      cout << "Error: could not open output file: " << string(outfile) << endl;
      exit(1);
    }
    long int bw = fwrite(&dd2[0], sizeof(float), NS, f);
    fclose(f);
    writeHeader((string(outfile) + string(".hdr")).c_str(), NR2, NC2, NBand);

}

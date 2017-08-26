#include "bikes.h"

void r4_f2( float t, float y[], float yp[] ){
	if(theSimulation)
		theSimulation->myr4_f2 (t, y, yp );
}

void * threadfun( void *arg){
	printf("threadfun\n");
	if(theSimulation){
		return( theSimulation->myThreadfun(arg));
	}else{
		printf("Error: simulation object was not yet defined.\n");
		return (void *) NULL;
		}
}


int main( int argc, char * argv[]){
	nBikes = NUMBIKES;
	myPylonInterval = 10.;
	nPylons = 100;
	roadWidth = 20.;
	arrowFactor = 8.;
	PAUSE_FLAG = false;//true;
	toTrack = -1;
	textureflag = ready = false;
	ready = true;
	TEX_WIDTH= TEX_HEIGHT = 2048;
	ui_image_copy = 0;
	tex_param_s  = 1.0f;
	tex_param_t  = 1.0f;
	tex_mat = 0;
	theSimulation = NULL;

	zprManager * myManager = zprManager::Instance(argc,argv);
	zprInstance * myZpr = myManager->newZprInstance(300,300,3);
	theSimulation = new odeSIM(myZpr);

//	myLinePlot * myLine = new myLinePlot( myManager);
//	myLine->setRightOf( myZpr);
//	myLine->forceUpdate = true;
	
	glutMainLoop();

	return 0;
}
odeSIM::odeSIM(zprInstance * myZpr_){
	rX = rY = rZ = 0;
	myZpr = myZpr_;
	neqn=nBikes*4;
	this->myBikes = new std::vector<glBike *>();
	this->mySpheres = new std::vector<glBasicSphere *>();
	this->initName(myZpr, false);

	this->initODE(); 
	this->initIC(); 
	pthread_create(&thread, NULL, &threadfun, NULL);
}
/* was odeSIM::odeSim * Instance */
odeSIM * Instance(zprInstance * myZpr_){
	if(!theSimulation){
	    theSimulation = new odeSIM(myZpr_);
	 }
	return(theSimulation);
    }
    //void initODE();

void odeSIM::myr4_f2 ( float t, float y[], float yp[] )
    //****************************************************************************80
    //    R4_F2 evaluates the derivative for the ODE.////  Parameters:
    //    Input, float T, the value of the independent variable.
    //    Input, float Y(NEQN), the value of the dependent variable.
    //    Output float YP(NEQN), the value of the derivative dY(1:NEQN)/dT.
    //
    {
	float myDistanceThreshold = (float) DRIVER_REPULSION_DISTANCE_THRESHOLD;
	float myDistanceThresholdRadius = (float) DRIVER_REPULSION_DISTANCE_THRESHOLD_RADIUS;
	int i;  
	for(i=0; i<nBikes; i++){
	    float x, yy;
	    x = y[4*i ];   yy =y[4*i+2];
	    if(yy<0) 
		yy=0.; 
	    if(yy>roadWidth) 
		yy=roadWidth;
	    y[4*i+2] =yy;
	}
	for(i=0; i<nBikes; i++){
	    float x,yy,Vx, Vy, xP, VxP, yP, VyP;
	    x  =  y[4*i ];   yy =y[4*i+2];
	    Vx = y[4*i+1];   Vy =y[4*i+3];
	    int j;
	    float interacX=0.; float interacY =0.; float nD =0.;
	    for(j=0; j<nBikes; j++){
			if(j==i) continue; 
			float d; float xY; float yY; 
			xY = y[4*j ]; yY = y[4*j+2];
			//d =sqrt( (x-xY)*(x-xY) + (yy-yY)*(yy-yY));  //distance to other rider. 
			float directionX = x  - xY; 
			float directionY = yy - yY; 
			myBikes->at(i)->vX->at(j) = directionX;
			myBikes->at(i)->vY->at(j) = directionY;
			d = sqrt( directionX*directionX + directionY*directionY); nD +=d;//=normDirection;
			//cout << "normDirection "<<normDirection<< "x="<<x<<","<<yy<<" y="<<xY <<"," <<yY<<" j "<<j<<endl;
			interacX += (1./(directionX/d))*Si( d, myDistanceThreshold, myDistanceThresholdRadius);
			interacY += (1./(directionY/d))*Si( d, myDistanceThreshold, myDistanceThresholdRadius);
	    }
	    //add repulsion from walls.. 
	    float dTerm1 = min(0., roadWidth-yy) ;
	    float dWall1 = (1./(yy-roadWidth))* Si( sqrt( (yy - roadWidth)*(yy-roadWidth)), 0., myDistanceThresholdRadius);
	    dWall1 = (isinf(dWall1) || isnan(dWall1)) ? 0. : dWall1; 
	    float dTerm2 = max(0., yy);
	    float dWall2 =  (1./yy)* Si( sqrt(yy*yy), 0., myDistanceThresholdRadius); 
		dWall2 = (isinf(dWall2) || isnan(dWall2))?  0. : dWall2; 
	
	    interacX = max(0., interacX);
	    float clamp = 155.;
	    dWall1 = min( clamp, dWall1); dWall1 = max(-clamp, dWall1);
	    dWall2 = min( clamp, dWall2); dWall2 = max(-clamp, dWall2);
	    interacX = min(clamp, interacX); interacX = max(-clamp, interacX);
	    interacY = min(clamp, interacY); interacY = max(-clamp, interacY);
	    interacX = (isinf(interacX)||isnan(interacX))?0.:interacX;
	    interacY = (isinf(interacY)||isnan(interacY))?0.:interacY;
	    myBikes->at(i)->interacX = 3.*interacX;  
	    myBikes->at(i)->interacY = 3.*interacY; 
	    myBikes->at(i)->dWall1   = 5.*dWall1;// = interacX; 
	    myBikes->at(i)->dWall2   = 5.*dWall2;
	    //myBikes->at(i)->arrow1x = 2.*interacX;
	    //myBikes->at(i)->arrow1y = 2.*interacY + (5.*(dWall1+dWall2));
		 
	    xP  = yp[4*i]   = Vx;                           // x'=Vx 
	    yP  = yp[4*i+2] = Vy;                  // y'=Vy;                        = y[4*i+3];
//FORCE TERM:
	    myBikes->at(i)->arrow1x  = 5. + myBikes->at(i)->interacX - abs( (float)(0.03*Vx*Vx*Vx));
			    VxP = yp[4*i+1] = myBikes->at(i)->arrow1x;//	    // (Vx)'/m=Fx
	    myBikes->at(i)->arrow1y  = 0. + myBikes->at(i)->interacY +(myBikes->at(i)->dWall1+myBikes->at(i)->dWall2);//+ 0.;
			    VyP = yp[4*i+3] = myBikes->at(i)->arrow1y;                        //  (Vy)'/m=Fx
	
	//printf("i %d x=(%f,%f) v=(%f,%f) fx %f fy %f nD %f intX %f intY %f dW1 %f dW2 %f \n",i,x,yy,Vx, Vy, interacX, interacY,nD, interacX, interacY, dWall1, dWall2);
	}
    return;
}
    
    void odeSIM::initBikes(){
//zprReferencePoint[0] = roadWidth; zprReferencePoint[1] = roadWidth/2.; zprReferencePoint[3]=0.; zprReferencePoint[4]=0.;
 // std::default_random_engine generator;
//  std::uniform_real_distribution<double> distribution(0.0,1.0);  
/*
std::uniform_real_distribution<double> unif;
std::random_device rd;
std::mt19937 engine(rd());
std::function<double ()> rnd = std::bind(unif, engine);  
*/
    int i;
    float myDistanceThreshold = (float) DRIVER_REPULSION_DISTANCE_THRESHOLD;
    float myDistanceThresholdRadius = (float) DRIVER_REPULSION_DISTANCE_THRESHOLD_RADIUS;

    for(i=0; i<nBikes; i++){
	float number = rnd();//distribution(generator);
	float number2 = rnd();
	cout <<"i " << number <<endl;
	glBike * myBike = new glBike(this, myZpr, 
	    0.05*roadWidth + number2*(0.95*roadWidth*((float)NUMBIKES/(6.*roadWidth))) ,  
	    0.05*roadWidth + number*(0.95*roadWidth),  0.,   0.,0.5,0.5,  
	    myDistanceThreshold/2.,MYSTACKS,MYSTACKS);//myCircles,myStacks
	myBikes->push_back(myBike);
    }
    
}

void odeSIM::initSpheres(){
    int i; 
    for(i=0; i<nPylons; i++){ // init pylon spheres.. 
	
	glBasicSphere * a = new glBasicSphere(i,myZpr,((float)i * myPylonInterval), 0,0,             0,0,1, 0.25,10,10);
	mySpheres->push_back(a);// a->arrow();
	glBasicSphere * b = new glBasicSphere(i,myZpr,((float)i * myPylonInterval),roadWidth,0,     0,0,1, 0.25,10,10);
	mySpheres->push_back(b);// b->arrow();
	a->setRelativePosition = 1;
	b->setRelativePosition =1;

    }
    //mySpheres->push_back(new glBasicSphere(2,2,2, 1,0,1, 0.25,10,10));
    //mySpheres->push_back(new glBasicSphere(0,0,0, 1,0,1, 0.25,10,10));
}
void odeSIM::drawSpheres(){
	/*
	return;
    std::vector<glBasicSphere *>::iterator it;
    for(it = mySpheres->begin(); it!=mySpheres->end(); it++){ (*it)->drawMe();}
    std::vector<glBike *>::iterator it2;
    for(it2 = myBikes->begin(); it2!=myBikes->end(); it2++){ (*it2)->drawMe();}
*/
}

void odeSIM::glAxes(){
	/*
	glPushMatrix();
	glEnable(GL_LINE_SMOOTH); glLineWidth(1.);
	glColor3f(0.05,0.5,0.1); 
	glBegin(GL_LINES); 
	    glVertex3f(0-rX,0-rY,0-rZ); 
	    glVertex3f(0-rX,roadWidth-rY,0-rZ); 
	glEnd();
	
	glColor3f(0.05,0.5,.1); glBegin(GL_LINES); 
	int k; for(k=0; k<nPylons; k++){
	    float dx =((float)k)*myPylonInterval;
	    glVertex3f(dx-rX ,0-rY,0-rZ); glVertex3f(dx-rX,roadWidth-rY,0-rZ); 
	}glEnd();
	glPopMatrix();	
	*/

}

void odeSIM::initSimulation(){
}


//void rebootODE();

/*void odeSIM::test02 ( );
void odeSIM::r4_f1 ( float t, float y[], float yp[] );
float odeSIM::r4_y1x ( float t ); //void r4_f2 ( float t, float y[], float yp[] );
void odeSIM::r8_f1 ( double t, double y[], double yp[] );
double odeSIM::r8_y1x ( double t );
void odeSIM::r8_f2 ( double t, double y[], double yp[] );
*/


void odeSIM::initIC(){
 int i;  
    for(i=0; i<nBikes; i++){
	y[4*i]  = *(myBikes->at(i)->x);  
	yp[4*i+1] =0.; // = y[4*i];    //x..=
	y[4*i+2] =*(myBikes->at(i)->y);// -y[4*i];     //x. =
	y[4*i+3] =0.;//1.;  // = y[4*i+3];  //y..=
//	yp[4*i+3] = -y[4*i+2];   //y.=
    }
}


void odeSIM::rebootODE(){
    abserr = sqrt ( r4_epsilon ( ) );
    relerr = sqrt ( r4_epsilon ( ) );
    flag = 1;
    t_start = 0.0;
    t_stop = 5.0 * 3.14159265;
    n_step = 10000;                    
    t_step = (t_stop-t_start) / (float)(n_step); 
    t = 0.0;
  t_out = 0.0;
  i_step = 0;
}

void odeSIM::drawTime(){
    sprintf(time_string, (!PAUSE_FLAG)?"%d hours %d minutes %3.2f seconds":"%d hours %d minutes %3.2f seconds (paused)", (int)floor(myTime/3600.), (int)floor( myTime/60.), myTime - 3600.*floor(myTime/3600.) -60.*floor(myTime/60.));   // char * str, const char * format, ... )

	printf("%s\n", time_string);
    setOrthographicProjection(myZpr->myWindowWidth, myZpr->myWindowHeight);
    glPushMatrix(); 
	
	if(!myZpr->isPaused){glColor3f(0.0f,1.0f,0.0f);}else{glColor3f(1.0f,0.0f,0.0f);}
    glLoadIdentity();
    int lightingState = glIsEnabled(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    if(DISPLAY_TIME) 
	renderBitmapString(3,10,(void *)MYFONT,time_string);
    if(lightingState) 
	glEnable(GL_LIGHTING);
    glPopMatrix();
    resetPerspectiveProjection();
}//y[4*i+1]


//https://computing.llnl.gov/tutorials/pthreads/
void * odeSIM::myThreadfun(void *arg){
// for ( i_step = 1; i_step <= n_step; i_step++ )
    while(true){
	while((++i_step)<=n_step){
		//printf("Step %d\n", i_step);
	    myTime = myTime + (((float)t_stop)-((float)t_start)) / ((float)n_step);
	    while(PAUSE_FLAG || myZpr->isPaused){
			//usleep(1000);
	    }
	    t = ( ( float ) ( n_step - i_step + 1 ) * t_start 
		+ ( float ) (          i_step - 1 ) * t_stop ) 
		/ ( float ) ( n_step              );
	    t_out = ( ( float ) ( n_step - i_step ) * t_start 
		    + ( float ) (	   i_step ) * t_stop ) 
		    / ( float ) ( n_step );
	    flag = -1;
	   flag = r4_rkf45 ( r4_f2, neqn, y, yp, &t, t_out, &relerr, abserr, flag );
	   int kk;
		for(kk=0; kk<nBikes; kk++){
			float x,yy;//,Vx, Vy, xP, VxP, yP, VyP;
			x  =  y[4*kk ];   yy =y[4*kk+2];
			*(myBikes->at(kk)->x)=x;
			*(myBikes->at(kk)->y)=yy;
			*(myBikes->at(kk)->z)=0.;
			(myBikes->at(kk)->mySphere->x).init(x,yy,0.);//=x;
			//(myBikes->at(kk)->mySphere->y)=yy;
			//(myBikes->at(kk)->mySphere->z)=0.;	    
			if( myBikes->at(kk)->mySphere->myName ==toTrack){
				rX = x; rY = 0.; /*rY =yy;*/ rZ = 0.; 
				cout << "toTrack "<< toTrack << " rX " << rX << " rY "<<rY << " rz "<<rZ<<endl;
			}
		}
	    renderflag = true;
		myZpr->mark();
		this->mark();
		this->Update = true;
		//myZpr->update= true;

	   // usleep(100);    //renderflag = true; printf("Sleeping\n");
	}
	rebootODE();
    }
}

void odeSIM::initODE(){
    initSpheres();
    initBikes();
    //PAUSE_FLAG=true; 
    DISPLAY_TIME = true;
    y = new float[neqn];
    yp = new float[neqn];
    rebootODE();
}

void odeSIM::drawMe(){
	printf("odeSIM::drawMe()\n");
	//this->glAxes();
	//this->drawSpheres();
	this->drawTime();
}




glBike::glBike(odeSIM * mySim, zprInstance * myZpr, float myX, float myY, float myZ, float myR, float myG, float myB, float mySize, int myCircles, int myStacks){
	mySIM = mySim;
	myZprInstance = myZpr;
	float myDistanceThreshold = (float) DRIVER_REPULSION_DISTANCE_THRESHOLD;
	float myDistanceThresholdRadius = (float) DRIVER_REPULSION_DISTANCE_THRESHOLD_RADIUS;
	float mySizeP = myDistanceThreshold/2.;// + (myDistanceThresholdRadius/2.);

	mySphere = new glBasicSphere(myZpr, myX,myY,myZ,myR,myG,myB,mySizeP,myCircles,myStacks);
	mySphere->setRelativePosition =1;
	x = &(mySphere->x.x);
	y = &(mySphere->x.y); 
	z = &(mySphere->x.z);
	arrow1x=arrow1y=arrow1z = 0.; arrow2x = arrow2y = arrow2z =0.;
	vX = new SA<float>( (int)NUMBIKES );
	vY = new SA<float>( (int)NUMBIKES );
	vZ = new SA<float>( (int)NUMBIKES );
}
void glBike::drawMe(){ 
	float rX = mySIM->rX; 
	float rY = mySIM->rY;
	float rZ = mySIM->rZ;
	/*if(mySphere->isPicked() && myZprInstance->myPickNames.size()==1){
		printf("Bike was picked.\n");
		mySIM->rX = mySphere->x.x;
		mySIM->rY = mySphere->x.y;
		mySIM->rZ = mySphere->x.z;
		//myZpr->rX = mySphere->x.x;
		//myZpr->rY = mySphere->x.y;
		//myZpr->rZ = mySphere->x.z;
		
		rX = mySIM->rX; 
		rY = mySIM->rY;
		rZ = mySIM->rZ;
		exit(1);
	}*/

	mySphere->drawMe();
	
	    glPushMatrix();  glEnable(GL_LINE_SMOOTH); glLineWidth(2.);

	    glColor3f(0.,1.,0.); 	    
	    glBegin(GL_LINES); 
	    glVertex3f( *x-rX, *y-rY, *z-rZ); 
	    glVertex3f( *x-rX + arrow1x, *y-rY + arrow1y, *z-rZ); 
	    glEnd();

	    glColor3f(1.,0.,0.); 	    
	    glBegin(GL_LINES); 
	    glVertex3f( *x-rX, *y-rY, *z-rZ); 
	    glVertex3f( *x -rX - interacX, *y -rY - interacY, *z-rZ); 
	    glEnd();

	    glColor3f(0.,0.,1.); 	    
	    glBegin(GL_LINES); 
	    glVertex3f( *x-rX, *y-rY, *z-rZ); 
	    glVertex3f( *x-rX , *y -rY + (dWall1+dWall2), *z-rZ); 
	    glEnd();
	    	    
	    glEnable(GL_LINE_SMOOTH); glLineWidth(0.1); glColor3f(0.1, .2, 0.3);
	    glBegin(GL_LINES); int i;
	    for(i=0; i<NUMBIKES; i++){
		glVertex3f( mySphere->x.x -rX, mySphere->x.y -rY, mySphere->x.z -rZ); 
		glVertex3f( mySphere->x.x - (vX->at(i)) -rX, mySphere->x.y - (vY->at(i)) -rY, mySphere->x.z -rZ); 
		
	    }glEnd();
	    glPopMatrix();
}


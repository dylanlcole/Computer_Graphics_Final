/*
 *  Lighting
 *
 *  Demonstrates basic lighting using a sphere and a cube.
 *
 Key bindings
  m          Toggles idle movement
  []         Lower/rise light
  </>        Changes idle when movement toggled
  x          Toggle light flicker
  arrows     Change view angle
  o/p        Change the time between light flicker
  w/s        move forward/back
  a/d        move left/right
  0          Reset view angle
  ESC        Exit
 */
#include "CSCIx229.h"
#include <stdlib.h>

int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=6.0;   //  Size of world

 double alpha = 0.6;  //alpha value

int interval = 10;  //time between flickers


int inc       =  10;  // Ball increment
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  50;  // Ambient intensity (%)
int diffuse   = 50;  // Diffuse intensity (%)
int specular  =   20;  // Specular intensity (%)
int shininess =   2;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   5;  // Elevation of light
int num = 15;
unsigned int texture[9]; // Texture names

double Px = 0; //movement vectors
double Py = 2;
double Pz = 1;


/*
    Takes in the values from vertexes and finds the cross product
    Optional parameter to flip the normal
*/
static void cross(double a1, double a2, double a3, double b1, double b2, double b3, double c1, double c2, double c3, int flip)
{
        double A1 = a1-c1;
        double A2 = a2-c2;
        double A3 = a3-c3;

        double B1 = b1 -c1;
        double B2 = b2-c2;
        double B3 = b3 - c3;

         double nX = A2*B3 - A3*B2;
         double nY = A3*B1 - A1*B3;
          double nZ = A1*B2 - A2*B1;
        if(flip == 0)
          glNormal3d(nX, nY, nZ);
         else
            glNormal3d(-nX, -nY, -nZ);


}

/*
    draws a gem at the given position, given size, and given number of sides on one face
*/

static void gem(double x, double y, double z, double dx, double dy, double dz, double sides, double rotateAngle, int opaque)
{
    float white[] = {0,0,0,0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(rotateAngle, 0, 0, 1);
   glScaled(dx,dy,dz);


      if (!opaque)  //enable blending
   {
      glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glDepthMask(0);
      glEnable(GL_TEXTURE_2D);
   }

 glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


    int k;
       int i;  //# of face
    int faces = 360/sides;


   for (i=1;i>=-1;i-=2)
   {
      glBegin(GL_TRIANGLES);


      for (k=0;k<=360-faces;k+=faces)  //draw triangles for number of faces given
      {

          //CROSS PRODUCT CALC
          double a1 = i*Cos(k);
          double a2 = Sin(k);
          double a3 = 0;
          double b1 = i*Cos(k+faces);
          double b2 = Sin(k+faces);
          double b3 = 0;
          double c1 = 0;
          double c2 = 0;
          double c3 = i;

          cross(a1, a2, a3, b1, b2, b3, c1, c2, c3, 0);
        glTexCoord2f(0.5, 0.5); glVertex3f(0,0,i);
        glTexCoord2f(0,0); glVertex3f(Cos(k),Sin(k), 0);
         glTexCoord2f(1,0);glVertex3f(Cos(k+faces),Sin(k+faces), 0);


      }
      glEnd();
   }


      if (!opaque)
   {
      glDisable(GL_BLEND);
      glDepthMask(1);
      glDisable(GL_TEXTURE_2D);
   }

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}


static void cylinder(double x, double y, double z, double r, double d, double rotateAngle)
{
    int k;
    float white[] = {0,0,0,0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(rotateAngle, 1, 0, 0);
   glScaled(r,r,d);

 glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


   //  Edge of Cylinder
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   glColor3f(1.0,1.0,1.0);
   glBegin(GL_QUADS);
   for (k=0;k<=360;k+=45)
   {
      glNormal3f(Cos(k),Sin(k),0);
               glTexCoord2f(0,0);glVertex3f(Cos(k),Sin(k), -1);
                glTexCoord2f(1,0);glVertex3f(Cos(k), Sin(k), 0);

                glTexCoord2f(1,1);glVertex3f(Cos(k+45), Sin(k+45), 0);
                glTexCoord2f(0,1);glVertex3f(Cos(k+45),Sin(k+45), -1);
   }
   glEnd();


      glBindTexture(GL_TEXTURE_2D,texture[3]); //edge texture
      glNormal3f(0,0,-1);
      glBegin(GL_TRIANGLE_FAN);  //draw faces
      glTexCoord2f(0.5,0.5);

            glVertex3f(0,0,-1);
      for (k=0;k<=360;k+=45)
      {
        glTexCoord2f(0,-1/2);

          glVertex3f(-1*Cos(k),Sin(k), -1);

      }
      glEnd();

    //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

static void triangle(double x, double y, double z, double base, double height, double thick, double rotateAngle)
{

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(rotateAngle, 0, 0, 1);
   glScaled(base,height,thick);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


   glBegin(GL_TRIANGLES); //face

       double i;
       for(i = -1; i <= 1; i+=2)
       {
        glNormal3f(0.0, 0.0, i);
       glTexCoord2f(0.5,1);glVertex3f(0.0, 1.0, 0.5*i);
       glTexCoord2f(0,0);glVertex3f(-1.0, 0.0, 0.5*i);
       glTexCoord2f(1,0);glVertex3f(1.0, 0.0, 0.5*i);
       }

   glEnd();

   glBegin(GL_QUADS); //edge of Triangle
        glNormal3f(0.0, -1.0, 0.0);
   glTexCoord2f(.9,0);glVertex3f(-1.0, 0.0,0.5);
   glTexCoord2f(0,0);glVertex3f(-1.0, 0.0,-0.5);
   glTexCoord2f(.9,0);glVertex3f(1.0, 0.0,-0.5);
   glTexCoord2f(.9,.9);glVertex3f(1.0, 0.0,0.5);

        glNormal3f(1.0, 1.0, 0.0);
   glTexCoord2f(.9,0);glVertex3f(1.0, 0.0,-0.5);
   glTexCoord2f(0,0);glVertex3f(1.0, 0.0,0.5);
   glTexCoord2f(.9,0);glVertex3f(0.0, 1.0,0.5);
   glTexCoord2f(.9,.9);glVertex3f(0.0, 1.0,-0.5);

        glNormal3f(-1.0, 1.0, 0.0);
   glTexCoord2f(.9,0);glVertex3f(0.0, 1.0,0.5);
   glTexCoord2f(0,0);glVertex3f(0.0, 1.0,-0.5);
   glTexCoord2f(.9,0);glVertex3f(-1.0, 0.0,-0.5);
   glTexCoord2f(.9,.9);glVertex3f(-1.0, 0.0,0.5);



    glEnd();

   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

/*
    calls triangles in order to make the triforce
*/

static void triforce(double x, double y, double z, double size, double rotateAngle)
{
            float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(rotateAngle, 0, 1, 0);
   glScaled(size,size,size);

    glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   glColor3f(1.00,0.77,0.36);
   glBindTexture(GL_TEXTURE_2D,texture[6]); //metal texture


   triangle(0,0.9999999,0,1,1,1,0);
   triangle(-1, 0,0, 1,1,1,0);
   triangle(1, 0,0, 1,1,1,0);

      //  Undo transformations
   glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}


/*
    draw the casing of the blue gem
*/
static void sapphireEdge(double x, double y, double z, double dx, double dy, double dz, double rotateAngle)
{
        float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,20);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(rotateAngle, 0, 0, 1);
   glScaled(dx,dy,dz);
       glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[6]);


   glBegin(GL_TRIANGLES);

int i;
for (i = 1; i >= -1; i-=2)
{
    cross(i*1,1,0,0,0,0,0,-0.3,i*-0.1, 0);

   glTexCoord2f(0.5,0.5);glVertex3f(i*1,1,0);
   glTexCoord2f(0,0);glVertex3f(0,0,0);
   glTexCoord2f(1,0);glVertex3f(0, 0.3, i*0.1);

   cross(i*1,1,0,0,0,0,0,0.3,i*-0.1, 0);

   glTexCoord2f(0.5,0.5);glVertex3f(i*1,1,0);
   glTexCoord2f(0,0);glVertex3f(0,0,0);
   glTexCoord2f(1,0);glVertex3f(0, 0.3, i*-0.1);

   glNormal3f(i*-1, 0.7, 0);


  glTexCoord2f(0.5,0.5); glVertex3f(i*1,1,0);
   glTexCoord2f(0,0);glVertex3f(0,0.3, 0.1);
   glTexCoord2f(1,0);glVertex3f(0,0.3, -0.1);
}

   glEnd();


   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

/*
    call of 10 sided gem and the edge for the gems
*/
static void sapphire(double x, double y, double z, double rotateAngle, int opaque)
{
        float white[] = {0,0,0,0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y+0.25,z);
   glRotated(rotateAngle, 0, 1, 0);
   glScaled(0.5,0.5,0.5);
       glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, texture[7]);

    if(!opaque)
    {

   glColor4f(0,0,1, alpha);

   gem(0,2,0,1,1,1,5,18, 0);
   gem(1.1,0.3,0,1,1,1,5,44, 0);
   gem(-1.1,0.3,0,1,1,1,5,-8, 0);
    }
    else{


   glColor3f(1.00,0.77,0.36);
   glBindTexture(GL_TEXTURE_2D,texture[6]);


   triangle(0,1.2,0,0.7,1.2,0.5,180);
   sapphireEdge(0.55, 1.15, 0, 0.9,1.3,2,-60);
   sapphireEdge(-0.55, 1.15, 0, 0.9,1.3,2,60);
   sapphireEdge(0, 0.4, 0, 0.9,1.3,2,180);
   gem(1.05, 1.45, 0, 0.3, 0.3, 0.3, 5, 0, 1);
   gem(-1.05, 1.45, 0, 0.3, 0.3, 0.3, 5, 0, 1);
   gem(0, -0.2, 0, 0.3, 0.3, 0.3, 5, 18, 1);

   glBegin(GL_TRIANGLES);

   glEnd();

    }

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

/*
    draws the edge of the ruby
*/

static void rubyEdge(double x, double y, double z, double dx, double dy, double dz, double rotateAngle)
{
        float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,20);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(rotateAngle, 0, 1, 0);
   glScaled(dx,dy,dz);
       glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


   glColor3f(1.00,0.77,0.36);
   glBindTexture(GL_TEXTURE_2D,texture[6]);

   glBegin(GL_TRIANGLES);


    cross(1,1,0,0,0,0,0,-0.3,-0.1, 0);  //Begin drawing the lower part

  glTexCoord2f(0.5,0.5); glVertex3f(1,1,0);
   glTexCoord2f(1,0);glVertex3f(0,0,0);
   glTexCoord2f(0,0);glVertex3f(0, 0.3, 0.1);

   cross(1,1,0,0,0,0,0,0.3,-0.1, 0);

   glTexCoord2f(0.5,0.5);glVertex3f(1,1,0);
   glTexCoord2f(1,0);glVertex3f(0,0,0);
   glTexCoord2f(0,0);glVertex3f(0, 0.3, -0.1);

   glNormal3f(-1, 0.7, 0);


   glTexCoord2f(0.5,0.5);glVertex3f(1,1,0);
   glTexCoord2f(0,0);glVertex3f(0,0.3, 0.1);
   glTexCoord2f(1,0);glVertex3f(0,0.3, -0.1);



    cross(1, 1, 0, 0, 0.3, 0.1, 0.6, -0.9, -0.08, 0);
   glTexCoord2f(0.5,0.5);glVertex3f(1,1,0);
   glTexCoord2f(0,0);glVertex3f(0,0.3, 0.1);
   glTexCoord2f(1,0);glVertex3f(0.6, 0.9, 0.08);

cross(1, 1, 0, 0, 0.3, -0.1, 0.6, 0.9, -0.08, 0);


   glTexCoord2f(0.5,0.5);glVertex3f(1,1,0);
  glTexCoord2f(0,0); glVertex3f(0,0.3, -0.1);
  glTexCoord2f(1,0); glVertex3f(0.6, 0.9, -0.08);


glNormal3d(-0.6, 0.9, 0);
  glTexCoord2f(0.5,0.5); glVertex3f(1,1,0);
  glTexCoord2f(0,0); glVertex3f(0.6, 0.9, -0.08);
  glTexCoord2f(1,0); glVertex3f(0.6, 0.9, 0.08);


   glEnd();

   glBegin(GL_QUADS);  //draw the upper part of the edge


       glNormal3d(-1, 1, 0);

   glVertex3f(0.6, 0.9, -0.08);
   glVertex3f(0.6, 0.9, 0.08);
   glVertex3f(0,0.3, 0.1);
   glVertex3f(0,0.3, -0.1);


   glNormal3d(1, 1, 0);

   glVertex3f(-0.6, 0.9, -0.08);
   glVertex3f(-0.6, 0.9, 0.08);
   glVertex3f(0,0.3, 0.1);
   glVertex3f(0,0.3, -0.1);

   glEnd();


   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

/*
        gets a 8 sided gem and calls the rubyEdge
*/

static void ruby(double x, double y, double z, double rotateAngle, int opaque)
{
        float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y+0.5,z);
   glRotated(rotateAngle, 0, 1, 0);
   glScaled(0.50,0.50,0.50);
       glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   if(!opaque)
   {

   glBindTexture(GL_TEXTURE_2D, texture[7]);

    glColor4f(1,0,0, alpha);

   gem(0,0.9,0,1.3,1.8,0.8,4,0, 0);

   }
    else{

   glColor3f(1.00,0.77,0.36);
   glBindTexture(GL_TEXTURE_2D,texture[6]);



   rubyEdge(0, -1.5, 0, 1.8,3,3,0);
   rubyEdge(0, -1.5, 0, 1.8,3,3,180);

   glBegin(GL_TRIANGLES);

   glEnd();
    }

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

/*
    draws the edge for the emerald.  Most complex edge
*/

static void emeraldEdge(double x, double y, double z, double dx, double dy, double dz, double rotateAngle)
{
        float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,20);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(rotateAngle, 0, 0, 1);
   glScaled(dx,dy,dz);
       glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


   glColor3f(1.00,0.77,0.36);
   glBindTexture(GL_TEXTURE_2D,texture[6]);


     int theta;
    double r1;
    double r2;

    double zr1;
    double zr2;


    glBegin(GL_QUAD_STRIP);


    zr1 = 0.7;
    zr2 = 0.7;

    //draws the flat base
    for(theta = 0; theta <= 540; theta++)
    {
        r1 = theta*0.005;
        r2 = (theta)*0.005;

    if(theta %6 == 0 && zr1 >=0)
    {

        zr1 -= 0.01;
        zr2 -= 0.01;
    }

        cross(r1*Cos(theta)/2, -r1*Sin(theta)/2, zr1, r2*Cos(theta+100)/2, -r2*Sin(theta+100)/2, zr2, r1*Cos(theta+1)/2, -r1*Sin(theta+1)/2, zr1, 0);
        glTexCoord2f(0.5,0);glVertex3f(r1*Cos(theta)/2, -r1*Sin(theta)/2, zr1);
        glTexCoord2f(0.5,0.5);glVertex3f(r2*Cos(theta+100)/2, -r2*Sin(theta+100)/2, zr2);

    }

    glEnd();

        glBegin(GL_QUAD_STRIP);

    //draws the 3d Part of the edge
    zr1 = 0.7;
    zr2 = 0.9;
    for(theta = 0; theta <= 540; theta++)
    {
        r1 = theta*0.005;
        r2 = (theta)*0.005;

    if(theta %6 == 0 && zr1 >=0)
    {

        zr1 -= 0.01;
        zr2 -= 0.01;
    }

        cross((r1*Cos(theta)/2), (-r1*Sin(theta)/2), zr1, (r2*Cos(theta+50)/2), (-r2*Sin(theta+50)/2), zr2, (r1*Cos(theta+1)/2), (-r1*Sin(theta+1)/2), zr1, 0);

       glTexCoord2f(0.5,0); glVertex3f(r1*Cos(theta)/2, -r1*Sin(theta)/2, zr1);
        glTexCoord2f(0.5,0.5);glVertex3f(r2*Cos(theta+50)/2, -r2*Sin(theta+50)/2, zr2);


    }

    glEnd();

    glBegin(GL_QUAD_STRIP);

    zr1 = 0.9;
    zr2 = 0.7;


    for(theta = 0; theta <= 540; theta++)
    {
        r1 = theta*0.005;
        r2 = (theta)*0.005;

          if(theta %6 == 0 && zr2>=0)
    {

        zr1 -= 0.01;
        zr2 -= 0.01;
    }

        cross((r1*Cos(theta+50)/2), (-r1*Sin(theta+50)/2), zr2, (r2*Cos(theta+100)/2), (-r2*Sin(theta+100)/2), zr1, (r1*Cos(theta+51)/2), (-r1*Sin(theta+51)/2), zr2, 0);

        glTexCoord2f(0.5,0);glVertex3f(r1*Cos(theta+50)/2, -r1*Sin(theta+50)/2, zr1);
        glTexCoord2f(0.5,0.5);glVertex3f(r2*Cos(theta+100)/2, -r2*Sin(theta+100)/2, zr2);


    }

    glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

/*
    calls a 12 sided gem and the emerald edge
*/
static void emerald(double x, double y, double z, double rotateAngle, int opaque)
{
        float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y+1,z);
   glRotated(rotateAngle, 0, 1, 0);
   glScaled(0.75,0.75,0.75);
       glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   if(!opaque)
   {

   glBindTexture(GL_TEXTURE_2D, texture[7]);

    glColor4f(0,1,0, alpha);
   gem(0,0,0,1,1,0.7,6,30, 0);

   }
   else
   {

   glColor3f(1.00,0.77,0.36);
   glBindTexture(GL_TEXTURE_2D,texture[6]);

   emeraldEdge(0,0,0,1,1,1,135);  //90
   emeraldEdge(0,0,0,1,1,-1,135);

   }

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

/*
    Simple object to hold the gems
*/
static void altar(double x, double y, double z, double dx, double dy, double dz, double rotateAngle)
{
        float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,20);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(rotateAngle, 0, 1, 0);
   glScaled(dx,dy,dz);
       glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


   glColor3f(1,1,1);;
   glBindTexture(GL_TEXTURE_2D,texture[1]);

   glBegin(GL_QUADS);

int i;

for(i = 1; i >=-1; i-=2)
{
    glNormal3f(0,0,i*1);
   glTexCoord2f(0,0);glVertex3f(-4, 0, i*2);
   glTexCoord2f(0,1);glVertex3f(-4, 1.5, i*2); //front
   glTexCoord2f(1,1);glVertex3f(4, 1.5, i*2);
   glTexCoord2f(1,0);glVertex3f(4, 0, i*2);


    glNormal3f(i*-1, 0, 0);
   glTexCoord2f(0,0);glVertex3f(i*-4, 0, -2);
   glTexCoord2f(0,1);glVertex3f(i*-4, 1.5, -2);
   glTexCoord2f(1,1);glVertex3f(i*-4, 1.5, 2);  //sides
   glTexCoord2f(1,0);glVertex3f(i*-4, 0, 2);


   glNormal3f(i*-4, 0, 0); //sides
        glTexCoord2f(0,1); glVertex3f(i*-4, 3, -2);
        glTexCoord2f(0,0);glVertex3f(i*-4, 1.5, -2);
        glTexCoord2f(1,0);glVertex3f(i*-4, 1.5, 0);
        glVertex3f(i*-4, 1.5, 0);

}

    glNormal3f(0,0,-1);   //back
    glTexCoord2f(0,0);glVertex3f(4, 1.5, -2);
    glTexCoord2f(0,1);glVertex3f(4, 3, -2);
    glTexCoord2f(1,1);glVertex3f(-4, 3, -2);
    glTexCoord2f(1,0);glVertex3f(-4, 1.5, -2);


        glEnd();

        glBindTexture(GL_TEXTURE_2D,texture[2]);
        glBegin(GL_QUADS);


    glNormal3f(0,1,0);

   glTexCoord2f(0,0); glVertex3f(-4, 1.5, 2); //flat top
    glTexCoord2f(0,1);glVertex3f(-4, 1.5, 0);
    glTexCoord2f(1,1);glVertex3f(4, 1.5, 0);
    glTexCoord2f(1,0);glVertex3f(4, 1.5, 2);

    glNormal3f(0, 1.5, 2);

    glTexCoord2f(0,0);glVertex3f(-4, 1.5, 0); //angle top
    glTexCoord2f(0,1);glVertex3f(-4, 3, -2);
    glTexCoord2f(1,1);glVertex3f(4, 3, -2);
    glTexCoord2f(1,0);glVertex3f(4, 1.5, 0);

    glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void wallcylinder(double x, double y, double z, double r, double d, double rotateAngle)
{
    int k;
    float white[] = {0,0,0,0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(rotateAngle, 0, 0, 1);
   glScaled(r,1,r);

 glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


   //  Edge
   glBindTexture(GL_TEXTURE_2D,texture[1]);
   glColor3f(1.0,1.0,1.0);

int j;

for(j = 1; j <= d; j+=2)
{
    glBegin(GL_QUADS);
   for (k=0;k<=360;k+=45)
   {
      glNormal3f(Cos(k),0,Sin(k));
               glTexCoord2f(0,0);glVertex3f(Cos(k),j, Sin(k));
                glTexCoord2f(0,0.5);glVertex3f(Cos(k), j-2, Sin(k));

                glTexCoord2f(0.5,0.5);glVertex3f(Cos(k+45), j-2, Sin(k+45));
                glTexCoord2f(0.5,0);glVertex3f(Cos(k+45),j, Sin(k+45));
   }
   glEnd();
}
    //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

/*
    drawing the edge for the seal which is around the triforce
*/

static void triforceEdge(double x, double y, double z, double size, double rotateAngle)
{
            float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(rotateAngle, 0, 1, 0);
   glScaled(size,size,size);

    glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   glColor3f(1.00,0.77,0.36);
   glBindTexture(GL_TEXTURE_2D,texture[6]);


   double theta;
   double width = 1.001;

double i;
double j;
for(j = 1; j >= -1; j-=2)  //drawing the face of the curve
{

for(i = 1; i >= -1; i-=2)
{
    glBegin(GL_QUAD_STRIP);
    for(theta = 0; theta <= 180; theta++)
   {
       glNormal3f(0,0,i);
       if(theta <= 90)
       {

       glTexCoord2f(0.5,0);glVertex3f(Sin(theta), Cos(theta),i/2);
       glTexCoord2f(0.5,0.5);glVertex3f(Sin(theta)/width, Cos(theta)/width, i/2);

       width = width + 0.011111;
       }
       else
       {
        glTexCoord2f(0.5,0);glVertex3f(Sin(theta), Cos(theta), i/2);
       glTexCoord2f(0.5,0.5);glVertex3f(Sin(theta)/width, Cos(theta)/width, i/2);

       width = width - 0.011111;
       }
   }
   glEnd();
}



glBegin(GL_QUAD_STRIP);
for(theta = 0; theta <=180; theta++)  //draw the edge of the outer section
{
    glNormal3f(Sin(theta), Cos(theta), 0);
    glTexCoord2f(0.5,0);glVertex3f(Sin(theta), Cos(theta), -0.5);
    glTexCoord2f(0.5,0.5);glVertex3f(Sin(theta), Cos(theta), 0.5);
}

glEnd();


glBegin(GL_QUAD_STRIP); //draw the edge of the inner section

width = 1.001;

for(theta = 0; theta <=180; theta++)
{
    cross(-Sin(theta)/width, -Cos(theta)/width, -0.5, -Sin(theta)/width, -Cos(theta)/width, 0.5, -Sin(theta+1)/width, -Cos(theta+1)/width, -0.5, 0);
    //glNormal3f(-Sin(theta), -Cos(theta), 0);
    glTexCoord2f(0.5,0);glVertex3f(Sin(theta)/width, Cos(theta)/width, -0.5);
    glTexCoord2f(0.5,0.5);glVertex3f(Sin(theta)/width, Cos(theta)/width, 0.5);

    if(theta <= 90)
    {
        width = width + 0.011111;
    }else{
    width = width - 0.011111;
    }

}
glEnd();
}

//Begin drawing the triangle edges

for(i = 1; i >= -1; i-=2)
{

    for(theta = 18; theta <=180;theta+=36)
    {

        glBegin(GL_TRIANGLES);

        glNormal3f(0,0,i*1);
       glTexCoord2f(0,0); glVertex3f(Sin(theta), Cos(theta), i*0.5);
        glTexCoord2f(0.5,0.5);glVertex3f(2*Sin(theta+9), 2*Cos(theta+9), i*0.10);
        glTexCoord2f(1,0);glVertex3f(Sin(theta+18), Cos(theta+18), i*0.5);

        glEnd();

        glBegin(GL_QUADS);

        cross(Sin(theta), Cos(theta), 0.5, Sin(theta), Cos(theta), -0.5, 2*Sin(theta+9), 2*Cos(theta+9), -0.5, 1);
        glTexCoord2f(1,0);glVertex3f(Sin(theta), Cos(theta), 0.5);
        glTexCoord2f(0,0);glVertex3f(Sin(theta), Cos(theta), -0.5);
        glTexCoord2f(1,1);glVertex3f(2*Sin(theta+9), 2*Cos(theta+9), -0.10);
        glTexCoord2f(0,1);glVertex3f(2*Sin(theta+9), 2*Cos(theta+9), 0.10);

        glEnd();
         glBegin(GL_QUADS);
         cross(Sin(theta+18), Cos(theta+18), 0.5,Sin(theta+18), Cos(theta+18), -0.5, 2*Sin(theta+9), 2*Cos(theta+9), -0.5, 0);
        glTexCoord2f(1,0);glVertex3f(Sin(theta+18), Cos(theta+18), 0.5);
        glTexCoord2f(0,0);glVertex3f(Sin(theta+18), Cos(theta+18), -0.5);
       glTexCoord2f(1,1); glVertex3f(2*Sin(theta+9), 2*Cos(theta+9), -0.10);
       glTexCoord2f(0,1); glVertex3f(2*Sin(theta+9), 2*Cos(theta+9), 0.10);

        glEnd();
    }

}

      //  Undo transformations
   glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

/*
    get the triforce and the triforce edge to make the seal
*/

static void seal(double x, double y, double z, double size, double rotateAngle)
{
         float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(rotateAngle, 0, 1, 0);
   glScaled(size,size,size);
       glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


   triforce(0,2.5,0,0.75, 0);
   triforceEdge(1,3,0,1,0);
   triforceEdge(-1,3,0,1,180);
   sapphireEdge(0,2,0,1,1,6,180);



   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}
/*
    draw the door for the scene
*/
static void door(double x, double y, double z, double size, double rotateAngle)
{
         float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(rotateAngle, 0, 1, 0);
   glScaled(size,size,size);
       glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

          //  Enable Polygon Offset
     //glEnable(GL_POLYGON_OFFSET_FILL);
     //glPolygonOffset(1,1);

     glBindTexture(GL_TEXTURE_2D,texture[8]);

   //draw door
   glColor3f(1,1,1);

   float i;
   int j;

   for(i = 0; i < 4.5; i+=0.9)  //"y"
   {
       for(j = -3; j < 3; j++)  //x
       {

   glBegin(GL_QUADS);   //divides into polygons to have lighting

   glNormal3f(0,0,-1);

   glTexCoord2f((j+3)*0.16667,i*0.22222);glVertex3f(j,i, 0);
   glTexCoord2f((j+3)*0.16667,(i+0.9)*0.22222);glVertex3f(j,i+0.9, 0);
   glTexCoord2f((j+4)*0.16667,(i+0.9)*0.22222);glVertex3f(j+1,i+0.9, 0);
   glTexCoord2f((j+4)*0.16667,i*0.22222);glVertex3f(j+1,i, 0);
   glEnd();

       }
    }

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}




static void room()
{

       int i;
       int j;
     float white[] = {0,0,0,0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

   glPushMatrix();
   //  Offset and scale
   glTranslated(0,0,0);


   //  Draw the rug
       glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


glEnable(GL_POLYGON_OFFSET_FILL);
     glPolygonOffset(2,2);

    glBindTexture(GL_TEXTURE_2D,texture[4]);

     glColor3f(1 ,1 ,1);


for(i = -10; i < 15; i+=5)
{
    for(j = -4; j < 4; j++)  //divide into multiple quads to have lighting
    {

     glBegin(GL_QUADS);

     glNormal3f(0,1,0);



     glTexCoord2f( (i+10)*0.04  ,(j+4)*0.125);glVertex3f(j, 0.05, i);
    glTexCoord2f((i+10)*0.04 ,(j+5)*0.125); glVertex3f(j+1, 0.05, i);
    glTexCoord2f((i+15)*0.04 ,(j+5)*0.125); glVertex3f(j+1, 0.05, i+5);
    glTexCoord2f((i+15)*0.04 ,(j+4)*0.125);glVertex3f(j, 0.05, i+5);

     glEnd();
    }
}


    glBindTexture(GL_TEXTURE_2D,texture[5]); //draw roof

    glBegin(GL_QUADS);

    glNormal3f(0,-1,0);

     glTexCoord2f(0,0.1);glVertex3f(-15, 15, -15);
    glTexCoord2f(1,0.1); glVertex3f(+15, 15, -15);
    glTexCoord2f(1,0.9); glVertex3f(+15, 15, +15);
    glTexCoord2f(0,0.9);glVertex3f(-15, 15, +15);
    glEnd();



   glBindTexture(GL_TEXTURE_2D,texture[0]);

   glColor3f(1,1,1);


   glBegin(GL_QUADS);


   for (i=-num;i<num;i++)
   {

      for (j=-num;j<num;j++)
      {
          glNormal3f(0,1,0);


          glTexCoord2f(0,0);glVertex3f(i, 0, j);
          glTexCoord2f(1,0);glVertex3f((i+1), 0, j);
          glTexCoord2f(1,1);glVertex3f((i+1), 0, (j+1));  //draws floor
          glTexCoord2f(0,1);glVertex3f(i, 0, (j+1));

      }

   }
   glEnd();


   glBindTexture(GL_TEXTURE_2D,texture[1]); //wall
    glColor3f(1,1,1);
   glBegin(GL_QUADS);



   for(i = -num; i < num; i+=5)
   {
             for(j = 0; j < num; j+=5) //draws walls
      {
          glNormal3f(0,0,1);

           glTexCoord2f(0,0);glVertex3f(i, j, -num);
          glTexCoord2f(1,0);glVertex3f((i+5), j,-num);
          glTexCoord2f(1,1);glVertex3f((i+5),(j+5),-num);
          glTexCoord2f(0,1);glVertex3f(i,(j+5),-num);

          glNormal3f(0,0,-1);

           glTexCoord2f(0,0);glVertex3f(i, j, num);
          glTexCoord2f(1,0);glVertex3f((i+5), j,num);
          glTexCoord2f(1,1);glVertex3f((i+5),(j+5),num);
          glTexCoord2f(0,1);glVertex3f(i,(j+5),num);

          glNormal3f(-1, 0,0);

          glTexCoord2f(0,0);glVertex3f(num, j,i);
          glTexCoord2f(1,0);glVertex3f(num, j, (i+5));
          glTexCoord2f(1,1);glVertex3f(num, (j+5),(i+5));
          glTexCoord2f(0,1);glVertex3f(num,(j+5), i);

          glNormal3f(1,0,0);

          glTexCoord2f(0,0);glVertex3f(-num, j,i);
          glTexCoord2f(1,0);glVertex3f(-num, j, (i+5));
          glTexCoord2f(1,1);glVertex3f(-num, (j+5),(i+5));
          glTexCoord2f(0,1);glVertex3f(-num,(j+5), i);
      }
   }



   glEnd();
   glDisable(GL_POLYGON_OFFSET_FILL);


int k;

for(k = 1; k >= -1; k-=2) //draws effect on back wall
{

   wallcylinder(5,0,k*-num, 0.75, num, 0);
   wallcylinder(-5,0,k*-num, 0.75, num, 0);

   wallcylinder(10,0,k*-num, 0.50, num, 0);
   wallcylinder(-10,0,k*-num, 0.50, num, 0);

   wallcylinder(4.2,7,k*-num, 0.25, 9, 25);
   wallcylinder(-4.2,7,k*-num, 0.25, 9, -25);

   wallcylinder(3.7,7,k*-num, 0.25, 5, 45);
   wallcylinder(-3.7,7,k*-num, 0.25, 5, -45);

   wallcylinder(5.35,5,k*-num, 0.4, num, -45);
   wallcylinder(-5.35,5,k*-num, 0.4, num, 45);

   wallcylinder(5.4,5,k*-num, 0.4, num, -75);
   wallcylinder(-5.4,5,k*-num, 0.4, num, 75);

   wallcylinder(5.35,5,k*-num, 0.4, 11, -25);
   wallcylinder(-5.35,5,k*-num, 0.4, 11, 25);
}

    door(0,0,15,1,0);

    seal(0,4,15,0.9,0);

    altar(0,0,-10,1,1,1,0);

   triforce(0,7,-14,1,0);

    int l;
 for(l = -1; l <=1; l+=2)  //columns in the scene
 {

    cylinder(l*10,0,-10,2,15,90);
    cylinder(l*10,0,10,2,15,90);
    cylinder(l*10,0,0,2,15,90);
 }


    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    ErrCheck("room");

}



/*
 *  Draw vertex in polar coordinates with normal
 */


static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}


/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */

static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,0.3,0);
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */

int  h = 0;
void display()
{
   // h = glutGet(GLUT_ELAPSED_TIME); //time elapsed in milliseconds

    double test = 100-diffuse;

if(axes) //toggle light flicker
{

    if(h > interval)
    {
    test  = rand()%(100-diffuse);

    h = 0;
    }

}

   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position

      double Ex = -2*dim*Sin(th)*Cos(ph); //sphereical to cartisian
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);

    gluLookAt(Px, Py, Pz , Ex+Px,Ey+Py, -Ez+Pz , 0,Cos(ph),0);
            //first 3 where you are
            //second 3 what you look at
            //third 3 is what is up

   //  smooth shading
   glShadeModel(GL_SMOOTH);


        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*(diffuse+test) ,0.004*(diffuse+test) ,0.000*(diffuse+test) ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};

        //  Light position
        float Position1[] = {5,3,-3,1.0};
        float Position2[] = {-5,3,-3,1.0};
        float Position3[] = {-5,3,10,1.0};
        float Position4[] = {5,3,10,1.0};
        float Position5[] = {4*Cos(zh),ylight,-10+4*Sin(zh),1.0};

        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);

        ball(Position1[0],Position1[1],Position1[2],0.2);
        ball(Position2[0],Position2[1],Position2[2],0.2);
        ball(Position3[0],Position3[1],Position3[2],0.2);
        ball(Position4[0],Position4[1],Position4[2],0.2);
        ball(Position5[0],Position5[1],Position5[2],0.2);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);

        float vAmbientLightBright[4] = {0.5f, 0.5f, 0.5f, 1.0f};  //Global ambient light
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, vAmbientLightBright);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);

        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0

        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
        glEnable(GL_LIGHT3);
        glEnable(GL_LIGHT4);
        glEnable(GL_LIGHT5);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT1,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT1,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT1,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT1,GL_POSITION,Position1);

        glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION ,100/100.0);
      glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION   ,20/100.0);
      glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,10/100.0);
///Light 2
       glLightfv(GL_LIGHT2,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT2,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT2,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT2,GL_POSITION,Position2);

        glLightf(GL_LIGHT2,GL_CONSTANT_ATTENUATION ,100/100.0);
      glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION   ,20/100.0);
      glLightf(GL_LIGHT2,GL_QUADRATIC_ATTENUATION,10/100.0);
///Light 3
      glLightfv(GL_LIGHT3,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT3,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT3,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT3,GL_POSITION,Position3);

        glLightf(GL_LIGHT3,GL_CONSTANT_ATTENUATION ,100/100.0);
      glLightf(GL_LIGHT3,GL_LINEAR_ATTENUATION   ,20/100.0);
      glLightf(GL_LIGHT3,GL_QUADRATIC_ATTENUATION,10/100.0);
///Light 4
      glLightfv(GL_LIGHT4,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT4,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT4,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT4,GL_POSITION,Position4);

        glLightf(GL_LIGHT4,GL_CONSTANT_ATTENUATION ,100/100.0);
      glLightf(GL_LIGHT4,GL_LINEAR_ATTENUATION   ,20/100.0);
      glLightf(GL_LIGHT4,GL_QUADRATIC_ATTENUATION,10/100.0);
///Light 5
      glLightfv(GL_LIGHT5,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT5,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT5,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT5,GL_POSITION,Position5);

        glLightf(GL_LIGHT5,GL_CONSTANT_ATTENUATION ,100/100.0);
      glLightf(GL_LIGHT5,GL_LINEAR_ATTENUATION   ,30/100.0);
      glLightf(GL_LIGHT5,GL_QUADRATIC_ATTENUATION,10/100.0);


   //  Draw scene

wallcylinder(Position1[0],0,Position1[2],0.15, 3, 0);
wallcylinder(Position2[0],0,Position2[2],0.15, 3, 0);
wallcylinder(Position3[0],0,Position3[2],0.15, 3, 0);
wallcylinder(Position4[0],0,Position4[2],0.15, 3, 0);

   room();

int o;

double l = (glutGet(GLUT_ELAPSED_TIME)/1000.0)/2;
  int lh = fmod(90*l,360.0);

for(o = 1; o >=0; o--)
{
    if(th > 0)
    {
    sapphire(-3,2,-9,lh,o);
    emerald(0,2,-9,lh,o);
    ruby(3,2,-9,lh,o);
    }
    else if( th < 0)
    {
        ruby(3,2,-9,lh,o);
        emerald(0,2,-9,lh,o);
        sapphire(-3,2,-9,lh,o);
    }
    else
    {
    sapphire(-3,2,-9,lh,o);
    emerald(0,2,-9,lh,o);
    ruby(3,2,-9,lh,o);
    }
}



   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);

   glColor3f(1,1,1);

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d   interval=%d  Light Flicker=%s", th,ph, interval, axes ? "On" : "Off");

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   if(axes)
   {
    h++;
   }

   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th -= 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th += 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
   {
      if((ph+5) <90) //change back for orbital mode
      {
      ph += 5;
      }
   }
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
   {
      if((ph-5) > -90)  //change black for orbital mode
      {
      ph -= 5;
      }
   }
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(mode?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
   {
       th = 0;
       ph = 5;
   }
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   else if (ch == 'o')
   {
       if(interval != 5)
      interval-=5;
   }
   else if (ch == 'p')
   {
       if(interval !=100)
      interval+=5;
   }
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   else if (ch == 'a')
   {
            double tempZ = Pz;
            double tempX = Px;

         if((((Pz - Cos(th+90)) < 14) && (Pz - Cos(th+90)) > -14))
      Pz -= Cos(th+90);  //Cos(th)
        if((((Px - Sin(th+90)) < 14)&&(Px - Sin(th+90)) > -14))
      Px -= Sin(th+90);

    if(Pz > 14 || Pz  < -14)
        Pz = tempZ;
    if(Px > 14 || Px < -14)
        Px = tempX;


   }
   else if (ch == 'd')
   {
       double tempZ = Pz;
            double tempX = Px;

        if((((Pz + Cos(th)) < 14) && (Pz + Cos(th)) > -14))
      Pz += Cos(th+90);  //Cos(th)
        if((((Px + Sin(th)) < 14)&&(Px + Sin(th)) > -14))
      Px += Sin(th+90);

      if(Pz > 14 || Pz  < -14)
        Pz = tempZ;
    if(Px > 14 || Px < -14)
        Px = tempX;

   }
   else if (ch == 'w')
   {
       double tempZ = Pz;
            double tempX = Px;

        if((((Pz - Cos(th)) < 14) && (Pz - Cos(th)) > -14))
      Pz -= Cos(th);  //Cos(th)
        if((((Px - Sin(th)) < 14)&&(Px - Sin(th)) > -14))
      Px -= Sin(th);

      if(Pz > 14 || Pz  < -14)
        Pz = tempZ;
    if(Px > 14 || Px < -14)
        Px = tempX;

   }
   else if(ch== 's')
   {
       double tempZ = Pz;
            double tempX = Px;

         if((((Pz + Cos(th)) < 14) && (Pz + Cos(th)) > -14))
      Pz += Cos(th);  //Cos(th)
        if((((Px + Sin(th)) < 14)&&(Px + Sin(th)) > -14))
      Px += Sin(th);

            if(Pz > 14 || Pz  < -18)
        Pz = tempZ;
    if(Px > 14 || Px < -14)
        Px = tempX;

   }

   //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(mode?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_ALPHA);
   glutInitWindowSize(600,600);
   glutCreateWindow("Final Project: Dylan Cole");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load textures

   texture[0] = LoadTexBMP("floor.bmp");     //issue here with blue polygons
   texture[1] = LoadTexBMP("wall.bmp");
   texture[2] = LoadTexBMP("marble.bmp");
   texture[3] = LoadTexBMP("edge.bmp");
   texture[4] = LoadTexBMP("rug.bmp");
   texture[5] = LoadTexBMP("roof.bmp");
   texture[6] = LoadTexBMP("metal.bmp");
   texture[7] = LoadTexBMP("water.bmp");
   texture[8] = LoadTexBMP("door.bmp");

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}

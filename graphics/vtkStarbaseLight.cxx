/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkStarbaseLight.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


Copyright (c) 1993-1996 Ken Martin, Will Schroeder, Bill Lorensen.

This software is copyrighted by Ken Martin, Will Schroeder and Bill Lorensen.
The following terms apply to all files associated with the software unless
explicitly disclaimed in individual files. This copyright specifically does
not apply to the related textbook "The Visualization Toolkit" ISBN
013199837-4 published by Prentice Hall which is covered by its own copyright.

The authors hereby grant permission to use, copy, and distribute this
software and its documentation for any purpose, provided that existing
copyright notices are retained in all copies and that this notice is included
verbatim in any distributions. Additionally, the authors grant permission to
modify this software and its documentation for any purpose, provided that
such modifications are not distributed without the explicit consent of the
authors and that existing copyright notices are retained in all copies. Some
of the algorithms implemented by this software are patented, observe all
applicable patent law.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.


=========================================================================*/
#include <math.h>
#include "vtkStarbaseRenderer.h"
#include "vtkStarbaseLight.h"
#include "vtkLight.h"

// Description:
// Implement base class method.
void vtkStarbaseLight::Render(vtkRenderer *aren,int light_index)
{
  float	dx, dy, dz;
  float	color[3];
  int light_flag;
  int fd;
  vtkStarbaseRenderer *ren = (vtkStarbaseRenderer *)aren;
  
  light_flag = ren->GetLightSwitch();
  fd = ren->GetFd();
  
  // get required info from light
  color[0] = this->Intensity * this->Color[0];
  color[1] = this->Intensity * this->Color[1];
  color[2] = this->Intensity * this->Color[2];
  
  dx = this->FocalPoint[0] - this->Position[0];
  dy = this->FocalPoint[1] - this->Position[1];
  dz = this->FocalPoint[2] - this->Position[2];
  
  // define the light source
  if (!this->Positional)
    {
    light_source(fd, light_index, DIRECTIONAL,
		 color[0], color[1], color[2],
		 -dx, -dy, -dz);
    }
  else
    {
    light_source(fd, light_index, POSITIONAL,
		 color[0], color[1], color[2],
		 this->Position[0], this->Position[1], this->Position[2]);
    if (this->ConeAngle < 180.0)
      {
      light_model(fd, light_index, SPOT_LIGHT | CONE_LIGHT,
		  (int)this->Exponent, 1.0, this->ConeAngle,
		  dx, dy, dz);
      }
    light_attenuation(fd, light_index, 1, 
		      this->AttenuationValues[0],
		      this->AttenuationValues[1],
		      this->AttenuationValues[2]);
    }
  
  light_flag |= (0x0001 << light_index);
  vtkDebugMacro(<< "Defining light\n");
  
  // update the light switch
  light_switch(fd, light_flag);
  ren->SetLightSwitch(light_flag);
  
  vtkDebugMacro(<< "SB_light_switch: " << light_flag << "\n");
}


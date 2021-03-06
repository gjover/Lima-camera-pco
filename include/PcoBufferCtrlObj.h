/**************************************************************************
###########################################################################
 This file is part of LImA, a Library for Image Acquisition

 Copyright (C) : 2009-2011
 European Synchrotron Radiation Facility
 BP 220, Grenoble 38043
 FRANCE

 This is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 This software is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, see <http://www.gnu.org/licenses/>.
###########################################################################
**************************************************************************/
#ifndef PCOBUFFERCTRLOBJ_H
#define PCOBUFFERCTRLOBJ_H

#include "Pco.h"
#include "PcoCamera.h"

#include "HwBufferMgr.h"



struct stcAllocBuff {
		bool pcoAllocBufferDone;
		bool createEventsDone;
		
		SHORT	pcoAllocBufferNr[8];				// bufnrM buffer number allocated by PCO_AllocateBuffer
        WORD	*pcoAllocBufferPtr[8];			// buffer allocated by PCO_AllocateBuffer
        DWORD	dwPcoAllocBufferSize[8];			// buffer allocated by PCO_AllocateBuffer

		WORD	*limaAllocBufferPtr[8];			// buffer allocated by Lima
        DWORD	dwLimaAllocBufferSize[8];			// buffer allocated by Lima

		//HANDLE bufferEvent[8];
        HANDLE bufferAllocEvent[8];

        DWORD bufferAssignedFrameFirst[8];
        DWORD bufferAssignedFrameLast[8];
        int bufferReady[8];

};

namespace lima
{
  namespace Pco
  {
    class Camera;
    class SyncCtrlObj;
    class Interface;

	//int Camera::pcoGetError();

    class DLL_EXPORT BufferCtrlObj : public SoftBufferCtrlObj
    {
      friend class Interface;
	  friend class Camera;

      DEB_CLASS_NAMESPC(DebModCamera,"BufferCtrlObj","Pco");
	
    
	public:
      BufferCtrlObj(Camera *cam);
      void prepareAcq();
      void startAcq();
      //void getStatus(int &err,bool& exposing) {err = m_status,exposing = m_exposing;}
      void getStatus(int &err) {err = m_cam->pcoGetError();}
      //void setStatus(int status) {m_status = status;}

        //-------------------------------------------------------------  moved from taco
        
	  
        int _xferImag();
        int _xferImagMult();
		void * BufferCtrlObj::_getLimaBuffer(int lima_buffer_nb, Sync::Status &status);
		  
		bool _getRequestStop() { return m_requestStop;}
		void _setRequestStop(bool requestStop) { m_requestStop = requestStop;}
		void _pcoAllocBuffersFree();
		void _pcoAllocBuffersInfo(int &nr, DWORD &size);

	
	private:
		SoftBufferCtrlObj::Sync *m_bufferSync;
		Cond cond;
		Camera* m_cam;
		int _assignImage2Buffer(DWORD &dwFrameFirst, DWORD &dwFrameLast, DWORD dwRequestedFrames, int bufIdx, bool live_mode);
		
		void _pcoAllocBuffers(bool max = false);
		struct stcAllocBuff m_allocBuff;
		unsigned long	m_frames_per_buffer;
		//-------------------------------------------------------------

		HANDLE&      	m_handle;
		int        	m_frame[2];
		SyncCtrlObj* 	m_sync;
		bool m_requestStop;
		int m_ImageBufferSize;

	};
  }
}
#endif

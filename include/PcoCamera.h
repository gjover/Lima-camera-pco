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
#ifndef PCOCAMERA_H
#define PCOCAMERA_H
#include "Pco.h"
#include "Pco_errt.h"
#include "Debug.h"
#include "Constants.h"
#include "HwMaxImageSizeCallback.h"
//#include "PcoBufferCtrlObj.h"

#define DISABLE_ACQ_ENBL_SIGNAL

#define BUFF_VERSION 1024

//--------------------------------------- debug const for talk
#define DBG_BUFF           0x00000001
#define DBG_XFER2LIMA      0x00000002
#define DBG_LIMABUFF       0x00000004
#define DBG_EXP            0x00000008
#define DBG_XFERMULT       0x00000010
#define DBG_XFERMULT1      0x00000020
#define DBG_ASSIGN_BUFF    0x00000040
#define DBG_STATUS		   0x00000080


#define DBG_DUMMY_IMG      0x00000100
#define DBG_ROI            0x00001000
//---------------------------------------

#define KILOBYTE (1024LL)
#define MEGABYTE (KILOBYTE * KILOBYTE)
#define GIGABYTE (KILOBYTE * MEGABYTE)

#define PCO_EDGE_PIXEL_RATE_MIN 95000000
#define PCO_EDGE_PIXEL_RATE_MAX 286000000
#define PCO_EDGE_PIXEL_RATE_LOW 100000000
#define PCO_EDGE_PIXEL_RATE_HIGH 286000000
#define PCO_EDGE_WIDTH_HIGH 1920
#define PCO_EDGE_LUT_SQRT 0x1612
#define PCO_EDGE_LUT_NONE 0
#define PCO_EDGE_SLEEP_SHUTTER_MS 10000

#define PCO_CL_BAUDRATE_115K2	115200

#define PCO_BUFFER_NREVENTS 2
struct stcXlatCode2Str {
		int code;
		char *str;
};


#define PCO_MAXSEGMENTS 4

#define LEN_DUMP 128
char DLL_EXPORT *_hex_dump_bytes(void *obj, size_t lenObj, char *buff, size_t lenBuff);


struct stcFrame {
	BOOL	changed;
	unsigned int nb;
	unsigned int done;

	unsigned int ram_current;

	unsigned long next2read;


};

class ringLog {
        enum { bufferSize = 32 };
        struct data{        
                time_t timestamp;
                char str[bufferSize];
        };
        
   public:
        ringLog(int size);
        ~ringLog();
        int add(char *s);
        int size() {return m_size;};
        void dumpPrint(bool direction);
		int dump(char *s, int lgMax, bool direction);

private:
        int m_capacity;
        int m_size;
        int m_head;
        struct data *buffer;
};

struct stcTemp {
	short wCcd, wCam, wPower;
	short wMinCoolSet, wMaxCoolSet;
	short wSetpoint;
};


#define SIZEARR_stcPcoHWIOSignal 10
struct stcPcoData {
	PCO_General stcPcoGeneral;
	PCO_CameraType	stcPcoCamType;
	PCO_Sensor stcPcoSensor;
	PCO_Description	stcPcoDescription;	/* camera description structure */
	PCO_Timing stcPcoTiming;
	PCO_Storage stcPcoStorage;
	PCO_Recording stcPcoRecording;
	PCO_Single_Signal_Desc stcPcoHWIOSignalDesc[SIZEARR_stcPcoHWIOSignal];
	PCO_Signal stcPcoHWIOSignal[SIZEARR_stcPcoHWIOSignal];
	WORD wNrPcoHWIOSignal0;
	WORD wNrPcoHWIOSignal;
	unsigned long long debugLevel;

	DWORD dwPixelRateMax;

	char model[MODEL_TYPE_SIZE+1], iface[INTERFACE_TYPE_SIZE+1];
	//int	interface_type;

	PCO_SC2_CL_TRANSFER_PARAM clTransferParam;
	int pcoError;
        char pcoErrorMsg[ERR_SIZE+1];

	double	cocRunTime;		/* cam operation code - delay & exposure time & readout in s*/
	double	frameRate;
    WORD    activeRamSegment;				/* active ram segment */

      	//WORD		m_acq_mode;
      	bool		bExtTrigEnabled;
      	WORD		storage_mode;
      	WORD		recorder_submode;
		unsigned long	frames_per_buffer; 
        DWORD   dwRamSize;
        WORD    wPixPerPage;
        DWORD   dwMaxFramesInSegment[4];
        DWORD   dwSegmentSize[4];

        DWORD   dwValidImageCnt[4];
        DWORD   dwMaxImageCnt[4];

		char		camera_name[CAMERA_NAME_SIZE];
        char		sensor_type[64];

        
        WORD    wNowADC, wNumADC;
        unsigned int    maxwidth_step, maxheight_step;

        struct stcTemp temperature;

		unsigned int bytesPerPix;     //unsigned int depth;
		unsigned int bitsPerPix;     //unsigned int bits;
		unsigned int maxWidth;		// unsigned int xmax;		/* Max size */
		unsigned int maxHeight;        //unsigned int ymax;

		WORD bMetaDataAllowed, wMetaDataMode, wMetaDataSize, wMetaDataVersion;
		
		long msAcqRec, msAcqXfer, msAcqTout, msAcqTnow, msAcqAll;
		time_t msAcqRecTimestamp, msAcqXferTimestamp, msAcqToutTimestamp, msAcqTnowTimestamp;
		int trace_nb_frames;

		struct stcTraceAcq{
			DWORD nrImgRecorded;
			DWORD maxImgCount;
			int nrImgRequested;
			int nrImgRequested0;
			int nrImgAcquired;
			long msTotal, msRecord, msRecordLoop, msXfer, msTout;
			double msImgCoc;
			double sExposure, sDelay;
			time_t endRecordTimestamp;
			time_t endXferTimestamp;
			char *fnId;
			void init();
		} traceAcq;

		DWORD dwPixelRate, dwPixelRateRequested;
		double fTransferRateMHzMax;

		WORD wXResActual, wYResActual, wXResMax, wYResMax;
		WORD wLUT_Identifier, wLUT_Parameter;

		DWORD dwAllocatedBufferSize;
		int iAllocatedBufferNumber;
		int iAllocatedBufferNumberLima;
		bool bAllocatedBufferDone;
		bool bRollingShutter;

		char version[BUFF_VERSION];

		double min_exp_time, min_exp_time_err, step_exp_time;
		double max_exp_time, max_exp_time_err;
		double min_lat_time, min_lat_time_err, step_lat_time;
		double max_lat_time, max_lat_time_err;
		
		stcPcoData();

};

enum enumChange {
	Invalid, Valid, Changed,
};

enum enumPcoFamily {
	Dimax       = 1<<0, 
	Edge        = 1<<1, 
	EdgeGL      = 1<<2,
	EdgeRolling = 1<<3, 
	Pco2k       = 1<<4,
	Pco4k       = 1<<5,
};

enum enumPcoStorageMode {
	Fifo = 1, RecSeq, RecRing,
};

struct stcRoi {
	enumChange changed;	/* have values been changed ? */
	unsigned int x[2];	/* ROI min/max in x dir.(note starts at 1)*/
	unsigned int y[2];	/* ROI min/max in y dir.(note starts at 1)*/
	unsigned int xstep;	/* ROI granularity in x dir */
	unsigned int ystep;	/* ROI granularity in x dir */
};


struct stcBinning {
	enumChange	changed;		/* have values been changed ? */
	unsigned int x;			/* amount to bin/group x data.                 */
	unsigned int y;			/* amount to bin/group y data.                 */
};


namespace lima
{
  namespace Pco
  {
    class SyncCtrlObj;
    class VideoCtrlObj;
    class  DLL_EXPORT  Camera : public HwMaxImageSizeCallbackGen
    {
      friend class Interface;
	  friend class DetInfoCtrlObj;
      friend class SyncCtrlObj;
	  friend class RoiCtrlObj;
	  friend class BufferCtrlObj;

      DEB_CLASS_NAMESPC(DebModCamera,"Camera","Pco");
      public:
        Camera(const char *camPar);
        ~Camera();

        void 	startAcq();
        void	reset();

		HANDLE& getHandle() {return m_handle;}

        void getMaxWidthHeight(DWORD& width,DWORD& height){width = m_pcoData->maxWidth, height = m_pcoData->maxHeight;}
        void getArmWidthHeight(WORD& width,WORD& height){width = m_pcoData->wXResActual, height = m_pcoData->wYResActual;}
        void getBytesPerPixel(unsigned int& pixbytes){pixbytes = m_pcoData->bytesPerPix;}
		void getBitsPerPixel(WORD& pixbits){pixbits = (WORD) m_pcoData->bitsPerPix;}

		int getNbAcquiredFrames() const {return m_acq_frame_nb;}

        void getCameraName(std::string& name);

        char *talk(char *cmd);

        unsigned long pcoGetFramesMax(int segmentPco);

		unsigned long	pcoGetFramesPerBuffer() { return m_pcoData->frames_per_buffer; }
		double pcoGetCocRunTime() { return m_pcoData->cocRunTime; }
		double pcoGetFrameRate() { return m_pcoData->frameRate; }

		WORD pcoGetActiveRamSegment() {return m_pcoData->activeRamSegment;}

		SyncCtrlObj*	_getSyncCtrlObj() { return m_sync;}
		struct stcPcoData * _getPcoData() {return  m_pcoData; }
		char* _PcoCheckError(int err, int&error) ;
		int pcoGetError() {return m_pcoData->pcoError;}

		char *_pcoSet_RecordingState(int state, int &error);
		int dumpRecordedImages(int &nrImages, int &error);

		WORD _getCameraType() {return m_pcoData->stcPcoCamType.wCamType ; }
		bool _isCameraType(int tp);
		bool _isConfig(){return m_config; };
		void _pco_set_shutter_rolling_edge(int &error);
		void msgLog(char *s);

	private:
		SyncCtrlObj*	m_sync;

		std::string m_log;
        //char pcoErrorMsg[ERR_SIZE+1];

		struct stcPcoData *m_pcoData;

        HANDLE	m_handle;				/* handle of opened camera */
        bool m_cam_connected;
	
		int m_pcoError;

        struct stcBinning m_bin;
        struct stcRoi m_roi;
		Roi m_RoiLima;
		
		//struct stcSize m_size;


		int		m_acq_frame_nb;
		bool m_config;

        int PcoCheckError(int err);
        void _allocBuffer();

        char *_talk(char *cmd, char *output, int lg);

		char *_pcoSet_Trig_Acq_Mode(int &error);
		char *_pcoSet_Storage_subRecord_Mode(enumPcoStorageMode, int &error);
		char *_pcoSet_Exposure_Delay_Time(int &error, int ph);
		char *_pcoSet_Cameralink_GigE_Parameters(int &error);
		char *_pcoGet_Camera_Type(int &error);
		char *_pcoGet_TemperatureInfo(int &error);
		void _pco_GetPixelRate(DWORD &pixRate, int &error);
		void _presetPixelRate(DWORD &pixRate, int &error);

		//char *_pco_SetCameraSetup(DWORD dwSetup, int &error);
		bool _get_shutter_rolling_edge(int &error);
		void _set_shutter_rolling_edge(bool roling, int &error);

		void _init();
		void _init_edge();
		void _init_dimax();
		char *_prepare_cameralink_interface(int &error);
		char *_get_coc_runtime(int &error);
		char *_set_metadata_mode(WORD wMetaDataMode, int &error);

		bool _isValid_pixelRate(DWORD dwPixelRate);
		
		bool _isValid_Roi(const Roi &new_roi, Roi &fixed_roi);
		//bool _isValid_Roi(Roi &new_roi);
		void _set_Roi(const Roi &roi, int &error);
		void _get_Roi(Roi &roi);
		void _get_MaxRoi(Roi &roi);
		void _get_RoiSize(Size& roi_size);

		void _get_ImageType(ImageType& image_type);
		void _get_PixelSize(double& x_size,double &y_size);
		void _get_XYsteps(Point &xy_steps);
		void _set_ImageType(ImageType curr_image_type);
		void _get_DetectorType(std::string& det_type);
		void _get_MaxImageSize(Size& max_image_size);
		void _pco_GetHWIOSignal(int &error);
		void _pco_SetHWIOSignal(int sigNum, int &error);
		void _pco_initHWIOSignal(int mode, int &error);
		unsigned long long _getDebug(unsigned long long mask);

		ringLog *m_msgLog;

    };
  }
}


#endif

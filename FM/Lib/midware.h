#ifndef __MIDWARE_H
#define __MIDWARE_H

#if defined (__cplusplus)
extern "C" {
#endif

	
////////////////////////////////////////////BackLight Control
	BOOL bklite_ctrl(DWORD nlight);   //nlight: 0--100
	BOOL get_bklight(DWORD *plight);

////////////////////////////////////////////Camera Control
	void CamSwitch(UINT8 nVal);                   //nVal:0,1
	void CamPreview(BOOL bPrev, RECT *pRect);
	void CamPohto(TCHAR *pPath, RECT *pRect);
	
////////////////////////////////////////////HWkey Control	
	BOOL KeyLightCtrl(BOOL bOnOff);   //TRUE:power on; FALSE:power off

////////////////////////////////////////////Touch Calibrate	
	BOOL TouchPanelCalibrate(void);

////////////////////////////////////////////GPS Module
	BOOL OpenGpsPower(BOOL bOnOff);   //TRUE: power up; FALSE: power off

////////////////////////////////////////////GPRS Module
	BOOL OpenGprsPower(BOOL bOnOff);   //TRUE: power up; FALSE: power off

////////////////////////////////////////////CS42L52 
	BOOL GSM_IIS2AIN(UCHAR nadcvol, UCHAR npgavol);
	BOOL GSM_AIN2IIS(void);

	BOOL CS42L52_IIS2AIN(UCHAR nadcvol, UCHAR npgavol);
	BOOL CS42L52_AIN2IIS(void);

	BOOL RecordOn(void);
	BOOL RecordOff(void);

////////////////////////////////////////////system power control
	DWORD GetIOInStatus(DWORD index);  //indiex:1-7
	BOOL GetACCStatus(void);   	//TRUE:ACC ON; FALSE: ACC OFF. 
	BOOL SystemReset(void);
	BOOL SystemPowerOff(void);

////////////////////////////////////////////Get OS version
	BOOL GetOSVersion(TCHAR *buff, DWORD buffsize);  //buffsize>=50

////////////////////////////////////////////FM
	BOOL TunerInit(void);
	BOOL TunerSetFreq(UINT32 freq);
	BOOL TunerSeek(char direction, UINT32 *pFreq);

	BOOL SPK_Channel(UINT8 index);  //index: 1(FM), 2(System), 3(Phone)
	BOOL SPK_ChannelVol(UINT8 nvol);  //main channel nvol:0--0x1E



#if defined (__cplusplus)
}
#endif

#endif /* __MIDWARE_H */ 

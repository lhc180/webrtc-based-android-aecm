/**
 * These are wrappers of native webrtc echo cancellation mobile edition functions.
 *
 *@author billhoo E-mail: billhoo@126.com
 *@version 0.1 2013-3-8
 */
#include <jni.h>
#include <stdlib.h> // for NULL
#include <assert.h>
#include <echo_control_mobile.h>

/**
 * This function is a wrapper which wraps the WebRtcAecm_Create function in WebRtc echo_control_mobile.c
 * Allocates the memory needed by the AECM. The memory needs to be initialized
 * separately using the WebRtcAecm_Init() function.
 *
 * Inputs:
 *        NONE
 * Outputs:
 *        NONE
 * Returns:
 *         -1: error
 *         other values: created AECM instance handler.
 *
 */JNIEXPORT jint JNICALL
Java_com_android_webrtc_audio_MobileAEC_nativeCreateAecmInstance(JNIEnv *env,
		jclass jclazz) {
	void *aecmInstHandler = NULL;
	if (WebRtcAecm_Create(&aecmInstHandler) == -1)
		return -1;
	else
		return ((int) aecmInstHandler); //returns the pointer which points to created AECM instance to JAVA layer.
}

/**
 * This is a wrapper wraps WebRtcAecm_Free function in echo_control_mobile.c
 * This function releases the memory allocated by WebRtcAecm_Create().
 *
 * Inputs:
 *         aecmHandler - handler of the AECM instance created by nativeCreateAecmInstance()
 * Outputs:
 *         NONE
 * Returns         0: OK
 *                      -1: error
 *
 */
JNIEXPORT jint
JNICALL Java_com_android_webrtc_audio_MobileAEC_nativeFreeAecmInstance(
		JNIEnv *env, jclass jclazz, jint aecmHandler) {
	void *aecmInst = (void *) aecmHandler;
	if (aecmInst == NULL)
		return -1;
	int ret = WebRtcAecm_Free(aecmInst);
	aecmInst = NULL;
	return ret;
}

/**
 * This wrapper wraps the WebRtcAecm_Init() function in WebRtc echo_control_mobile.c
 * Initializes an AECM instance.
 *
 * Inputs:
 *            aecmHandler     - Handler of AECM instance
 *            sampFreq           - Sampling frequency of data
 * Outputs:
 *           NONE
 * Return:          0: OK
 *                       -1: error
 *
 */
JNIEXPORT jint
JNICALL Java_com_android_webrtc_audio_MobileAEC_nativeInitializeAecmInstance(
		JNIEnv *env, jclass jclazz, jint aecmHandler, jint sampFreq) {
	void *aecmInst = (void *) aecmHandler;
	if (aecmInst == NULL)
		return -1;
	return WebRtcAecm_Init(aecmInst, sampFreq);
}

/**
 * This wrapper wraps the WebRtcAecm_BufferFarend function in echo_control_mobile.c
 * Inserts an 80 or 160 sample block of data into the farend buffer.
 *
 * Inputs:
 *       aecmHandler    - Handler to the AECM instance
 *       farend               - In buffer containing one frame of farend signal for L band
 *       nrOfSamples    - Number of samples in farend buffer
 * Outputs:
 *       NONE
 * Return:     0: OK
 *                  -1: error
 *
 */
JNIEXPORT jint JNICALL Java_com_android_webrtc_audio_MobileAEC_nativeBufferFarend(
		JNIEnv *env, jclass jclazz, jint aecmHandler, jshortArray farend,
		jint nrOfSamples) {
	void *aecmInst = (void *) aecmHandler;
	if (aecmInst == NULL)
		return -1;

	int ret = -1;
	if (farend != NULL) {
		short *arrFarend = (*env)->GetShortArrayElements(env, farend, NULL);
		ret = WebRtcAecm_BufferFarend(aecmInst, arrFarend, nrOfSamples);

		//TODO(billhoo) should use JNI_ABORT instead of 0 in 4th param.
		//I think there is no need to copy this array back to Java layer.
		(*env)->ReleaseShortArrayElements(env, farend, arrFarend, 0);
	}
	return ret;
}

/**
 * This wrapper wraps the WebRtcAecm_Process in echo_control_mobile.c
 * Runs the AECM on an 80 or 160 sample blocks of data.
 *
 * Inputs:
 *         aecmHandler           - Handler to the AECM handler
 *         nearendNoisy          - In buffer containing one frame of reference nearend+echo signal.
 *                                             If noise reduction is active, provide the noisy signal here.
 *         nearendClean          -  In buffer containing one frame of nearend+echo signal.
 *                                             If noise reduction is active, provide the clean signal here.
 *                                             Otherwise pass a NULL pointer.
 *         nrOfSamples           - Number of samples in nearend buffer
 *         msInSndCardBuf    - Delay estimate for sound card and system buffers
 * Outputs:
 *         out    - Out buffer, one frame of processed nearend.
 * Return:     0: OK
 *                  -1: error
 *
 */
JNIEXPORT jint JNICALL Java_com_android_webrtc_audio_MobileAEC_nativeAecmProcess(
		JNIEnv *env, jclass jclazz, jint aecmHandler,
		const jshortArray nearendNoisy, const jshortArray nearendClean,
		jshortArray out, jshort nrOfSamples, jshort msInSndCardBuf) {

	int16_t *arrNearendNoisy = NULL;
	int16_t *arrNearendClean = NULL;
	int16_t *arrOut = NULL;

	void *aecmInst = (void *) aecmHandler;
	if (aecmInst == NULL)
		return -1;

	int ret = -1;

	//nearendNoisy and out must not be NULL, otherwise process can not be run, return -1 for error.
	if (nearendNoisy == NULL || out == NULL)
		return ret;

	//get data from java side.
	arrNearendNoisy = (*env)->GetShortArrayElements(env, nearendNoisy, NULL);
	arrOut = (*env)->GetShortArrayElements(env, out, NULL);

	if (nearendClean != NULL)
		arrNearendClean = (*env)->GetShortArrayElements(env, nearendClean,
				NULL);

	ret = WebRtcAecm_Process(aecmInst, arrNearendNoisy, arrNearendClean, arrOut,
			nrOfSamples, msInSndCardBuf);

	//release and send the changes back to java side.
	(*env)->ReleaseShortArrayElements(env, nearendNoisy, arrNearendNoisy, 0);
	(*env)->ReleaseShortArrayElements(env, out, arrOut, 0);

	if (nearendClean != NULL)
		(*env)->ReleaseShortArrayElements(env, nearendClean, arrNearendClean,
				0);

	return ret;
}

/**
 * This wrapper wraps the WebRtcAecm_set_config function in echo_control_mobile.c
 * Enables the user to set certain parameters on-the-fly.
 *
 * Inputs:
 *        aecHandler - Handler to the AEC instance.
 *        aecConfig - the new configuration of AEC instance to set.
 * Outputs:
 *         NONE
 * Return:     0: OK
 *                  -1: error
 *
 */
JNIEXPORT jint JNICALL Java_com_android_webrtc_audio_MobileAEC_nativeSetConfig(
		JNIEnv *env, jclass jclazz, jint aecmHandler, jobject aecmConfig) {

	void * aecmInst = (void *) aecmHandler;
	if (aecmInst == NULL)
		return -1;

	//get reference of AecmConfig class  from java side.
	jclass JavaAecmConfig = (*env)->GetObjectClass(env, aecmConfig);

	//assertion that class not be NULL
	//TODO(billhoo) should use Exception handler to handle this situation instead of assertion.
	assert(JavaAecmConfig != NULL);

	//get configuration field IDs from java side.
	jfieldID mAecmModeID = (*env)->GetFieldID(env, JavaAecmConfig, "mAecmMode",
			"S");
	jfieldID mCngModeID = (*env)->GetFieldID(env, JavaAecmConfig, "mCngMode",
			"S");

	//if any ID is NULL, return -1 for error.
	if (mAecmModeID == NULL || mCngModeID == NULL)
		return -1;

	//get values of fields
	short echoMode = (*env)->GetShortField(env, aecmConfig, mAecmModeID);
	short cngMode = (*env)->GetShortField(env, aecmConfig, mCngModeID);

	//set new configuration to AECM instance.
	AecmConfig config;
	config.echoMode = echoMode;
	config.cngMode = cngMode;

	return WebRtcAecm_set_config(aecmInst, config);
}

/*
 * This function enables the user to set certain parameters on-the-fly
 *
 * Inputs                       Description
 * -------------------------------------------------------------------
 * void *aecmInst               Pointer to the AECM instance
 *
 * Outputs                      Description
 * -------------------------------------------------------------------
 * AecmConfig  *config          Pointer to the config instance that
 *                              all properties will be written to
 * WebRtc_Word32  return        0: OK
 *                             -1: error
 */
//TODO(billhoo) wrap this into JNI interface if necessary.
int32_t WebRtcAecm_get_config(void *aecmInst, AecmConfig *config);

/*
 * This function enables the user to set the echo path on-the-fly.
 *
 * Inputs                       Description
 * -------------------------------------------------------------------
 * void*        aecmInst        Pointer to the AECM instance
 * void*        echo_path       Pointer to the echo path to be set
 * size_t       size_bytes      Size in bytes of the echo path
 *
 * Outputs                      Description
 * -------------------------------------------------------------------
 * WebRtc_Word32  return        0: OK
 *                             -1: error
 */
//TODO(billhoo) wrap this into JNI interface if necessary.
int32_t WebRtcAecm_InitEchoPath(void* aecmInst, const void* echo_path,
		size_t size_bytes);

/*
 * This function enables the user to get the currently used echo path
 * on-the-fly
 *
 * Inputs                       Description
 * -------------------------------------------------------------------
 * void*        aecmInst        Pointer to the AECM instance
 * void*        echo_path       Pointer to echo path
 * size_t       size_bytes      Size in bytes of the echo path
 *
 * Outputs                      Description
 * -------------------------------------------------------------------
 * WebRtc_Word32  return        0: OK
 *                             -1: error
 */
//TODO(billhoo) wrap this into JNI interface if necessary.
int32_t WebRtcAecm_GetEchoPath(void* aecmInst, void* echo_path,
		size_t size_bytes);

/*
 * This function enables the user to get the echo path size in bytes
 *
 * Outputs                      Description
 * -------------------------------------------------------------------
 * size_t       return           : size in bytes
 */
//TODO(billhoo) wrap this into JNI interface if necessary.
size_t WebRtcAecm_echo_path_size_bytes();

/*
 * Gets the last error code.
 *
 * Inputs                       Description
 * -------------------------------------------------------------------
 * void         *aecmInst       Pointer to the AECM instance
 *
 * Outputs                      Description
 * -------------------------------------------------------------------
 * WebRtc_Word32  return        11000-11100: error code
 */
//TODO(billhoo) wrap this into JNI interface if necessary.
int32_t WebRtcAecm_get_error_code(void *aecmInst);

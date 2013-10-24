package com.billhoo.android.aec.demo;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;

import com.android.webrtc.audio.MobileAEC;
import com.android.webrtc.audio.R;

/**
 * This demo will show you how to use MobileAEC class to deal with echo things
 * on android.<br>
 * </ul> <br>
 * <br>
 * <b>[NOTICE]</b>
 * <ul>
 * you should add <b>"android.permission.WRITE_EXTERNAL_STORAGE"</b> in your
 * AndroidManifest.xml file to allow this DEMO write data into your SDcard.
 * </ul>
 * <b>[TODO List]</b>
 * <ul>
 * <li>
 * (billhoo): should write all the demo processes into separate thread instead
 * of the UI thread.</li>
 * </ul>
 * 
 * @author billhoo E-mail: billhoo@126.com
 */
public class DemoActivity extends Activity {
	private static final boolean AECM_DEBUG = true;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		if (AECM_DEBUG) {
			doAECM();
		}
	}

	// //////////////////////////////////////////////////////////////////////////
	// ACOUSTIC ECHO CANCELLATION MOBILE EDITION

	public void doAECM() {
		try {
			MobileAEC aecm = new MobileAEC(null);
			aecm.setAecmMode(MobileAEC.AggressiveMode.MOST_AGGRESSIVE)
					.prepare();

			// get pcm raw data file in root of android sd card.
			// if you test this demo, you should put these files into your
			// android device or emulator.
			// the ideal output of pcm is almost down to zero.
			FileInputStream fin = new FileInputStream(new File(
					Environment.getExternalStorageDirectory()
							+ "/en-00-raw-pcm-16000Hz-16bit-mono.pcm"));

			FileOutputStream fout = new FileOutputStream(new File(
					Environment.getExternalStorageDirectory()
							+ "/aecm-output-pcm-16000Hz-16bit-mono.pcm"));

			final int cacheSize = 320;
			byte[] pcmInputCache = new byte[cacheSize];

			// core procession
			for (/* empty */; fin.read(pcmInputCache, 0, pcmInputCache.length) != -1; /* empty */) {
				// convert bytes[] to shorts[], and make it into little endian.
				short[] aecTmpIn = new short[cacheSize / 2];
				short[] aecTmpOut = new short[cacheSize / 2];
				ByteBuffer.wrap(pcmInputCache).order(ByteOrder.LITTLE_ENDIAN)
						.asShortBuffer().get(aecTmpIn);

				// aecm procession, for now the echo tail is hard-coded 10ms,
				// but you
				// should estimate it correctly each time you call
				// echoCancellation, otherwise aecm
				// cannot work.
				aecm.farendBuffer(aecTmpIn, cacheSize / 2);
				aecm.echoCancellation(aecTmpIn, null, aecTmpOut,
						(short) (cacheSize / 2), (short) 10);

				// output
				byte[] aecBuf = new byte[cacheSize];
				ByteBuffer.wrap(aecBuf).order(ByteOrder.LITTLE_ENDIAN)
						.asShortBuffer().put(aecTmpOut);

				fout.write(aecBuf);
			}

			fout.close();
			fin.close();
			aecm.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
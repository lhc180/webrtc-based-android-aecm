webrtc-based-android-aecm
=========================

Java API for android acoustic echo cancellation.

NOTE
=========================
  1. This post is a buggy one, the bug is when you run AECM on the same PCM file, the result is not true. I do fixed it several months ago, but after my SVN crashes, I have to remember how to fix it again, but now I have no time to do this.
  2. This post is based on WebRTC r3212, and I haven't update the core process of AECM module for several monthes.

TODO List
=========================
  1. Fix the bug mentioned above.
  2. Provide a VoIP demo to show how to use this API instead of doing AECM on PCM files.

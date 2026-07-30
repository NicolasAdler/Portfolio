# MATLAB Speech Recorder

This was a tool I developed during my time at Motorola. At the time I was tasked with capturing
speech which would later be fed into other voice testing pipelines to verify if certain denoising
algorithms were functioning properly. Before I could play the speech in certain noise conditions
using SoundCheck and then analyze the speech quality or Signal to Noise Ratio or Signal to Noise Ratio Improvement
I first needed to capture the original human speech. 

There was an existing MATLAB app that was being used but it had a few bugs, there was no way to
re-record bad takes, nor was there a way for the user to see how many lines left they needed to record. Users
could accidentally skip a line that needed to be recorded and since there was no way to go back, they would
need to start from the beginning again.

My MATLAB app fixed all of these issues and added in a word wrapper, which forced whole words to the next line
once the character count exceeded a threshold.

I test ran several tests with the old version of the MATLAB app and mine, and on average, my application
cut the time needed by each person to record each line by around 45%.

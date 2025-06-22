[in, fs] = audioread("flute.wav");
delayTime = 2.0;
delayTimeSamples = fix(fs * delayTime);
totalNumChannles = size(in,2);
totalNumSamples = size(in,1);

circularBufferLeft = zeros(totalNumSamples + delayTimeSamples,1);
circularBufferRight = zeros(totalNumSamples + delayTimeSamples,1);
outputBuffer = [circularBufferLeft, circularBufferRight];

read_pointer = 1;
write_pointer = 1;

for channel = 1:totalNumChannles
    for n = 1:totalNumSamples
        circularBufferLeft(n, 1) = in(n, 1);
        circularBufferRight(n, 1) = in(n, 2);
        read_pointer = write_pointer - delayTimeSamples;
        if(read_pointer < 1)
            read_pointer = read_pointer + size(circularBufferRight,1);
        end
        write_pointer = write_pointer + 1;
        read_pointer = read_pointer + 1;
        if (read_pointer >= size(circularBufferRight))
            read_pointer = 1;
        end
        if (write_pointer >= size(circularBufferRight))
            write_pointer = 1;
        end
        % if (channel == 1)
        %     outputBuffer(n, 1) =  circularBufferLeft(write_pointer, 1) + circularBufferLeft(read_pointer, 1);
        % end
        % if (channel == 2)
        %     outputBuffer(n, 2) =  circularBufferRight(write_pointer, 1) + circularBufferRight(read_pointer, 1);
        % end
        outputBuffer(n, 1) = in(n, 1) + circularBufferLeft(read_pointer);
        outputBuffer(n, 2) = in(n, 2) + circularBufferRight(read_pointer);
    end
end

soundsc(outputBuffer, fs);

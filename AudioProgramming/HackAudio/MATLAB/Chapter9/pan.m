function [out] = pan(in, panValue, panType)
    %in = in(:);
    panTransform = (panValue / 200) + 0.5;

    if panType == 1
        leftAmp = 1 - panTransform;
        rightAmp = panTransform;
    elseif panType == 2
        leftAmp = sqrt(1 - panTransform);
        rightAmp = sqrt(panTransform);
    elseif panType == 3
        leftAmp = sin((1 - panTransform) * (pi/2));
        rightAmp = sin(panTransform * (pi/2));
    end

    leftChannel = leftAmp .* in;
    rightChannel = rightAmp .* in;

    out = [leftChannel, rightChannel];
end

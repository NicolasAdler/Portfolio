function output = InsertNewLine(phrase, maxChars)

    if maxChars < 50
        maxChars = 50;
    end
    output = "";

    phraseLength  = strlength(phrase);
    startIdx = 1;

    while startIdx <= phraseLength

        endIdx = min(startIdx + maxChars - 1, phraseLength);
        if endIdx < phraseLength && phrase(endIdx + 1) ~= " "
            lastSpace = strfind(extractBefore(phrase, endIdx + 1), " ");
            if ~isempty(lastSpace)
                endIdx = lastSpace(end);
            end
        end

        output = output + extractBetween(phrase, startIdx, endIdx) + newline;

        startIdx = endIdx + 1;

        while startIdx <= phraseLength && phrase(startIdx) == " "

            startIdx = startIdx + 1;
        end
    end
end
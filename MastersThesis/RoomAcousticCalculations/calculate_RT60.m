function RT60 = calculate_RT60(ir, fs)
    
    schroeder_curve = flipud(cumsum(flipud(ir.^2))); 
    schroeder_dB = 10 * log10(schroeder_curve / max(schroeder_curve)); 
    schroeder_dB = sgolayfilt(schroeder_dB, 3, 11);

    t = (0:length(ir)-1) / fs;

    start_T20 = find(schroeder_dB <= -5, 1);
    end_T20 = find(schroeder_dB <= -25, 1);

    start_T30 = find(schroeder_dB <= -5, 1);
    end_T30 = find(schroeder_dB <= -35, 1);

    if ~isempty(start_T20) && ~isempty(end_T20)
        p_T20 = polyfit(t(start_T20:end_T20), schroeder_dB(start_T20:end_T20), 1);
        T20 = -60 / (3 * p_T20(1));
    else
        T20 = NaN;
    end

    if ~isempty(start_T30) && ~isempty(end_T30)
        p_T30 = polyfit(t(start_T30:end_T30), schroeder_dB(start_T30:end_T30), 1);
        T30 = -60 / (2 * p_T30(1));
    else
        T30 = NaN;
    end

    if ~isnan(T30)
        RT60 = T30 * 2;
    elseif ~isnan(T20)
        RT60 = T20 * 3;
    else
        RT60 = NaN;
    end

    % Display estimated RT60
    % fprintf('Estimated RT60: %.3f seconds\n', RT60);
    % 
    % % Plot Schroeder curve and linear fits
    % figure;
    % plot(t, schroeder_dB, 'b', 'LineWidth', 1.5); hold on;
    % if ~isnan(T20)
    %     plot(t(start_T20:end_T20), polyval(p_T20, t(start_T20:end_T20)), 'g--', 'LineWidth', 1.5);
    % end
    % if ~isnan(T30)
    %     plot(t(start_T30:end_T30), polyval(p_T30, t(start_T30:end_T30)), 'r--', 'LineWidth', 1.5);
    % end
    % xlabel('Time (s)');
    % ylabel('Energy Decay (dB)');
    % title('Schroeder Curve and RT60 Estimation');
    % grid on;
    % legend('Schroeder Curve', 'T20 Fit', 'T30 Fit');
end

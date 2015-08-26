function[] = PingPong(N, Bv, Kp, Ti, Td, dT)
%
% Funktionen m�jligg�r online-plot av en PID-reglering
% via Arduino Due samt justering av regleringsvariabler.
%
% Skapat den: 5 april 2015.
% Skapat av: Andreas Bj�rntoft & Karl M�senbacher.
%
% VARIABLER F�R REGLERING:
% N  = Antal samplingar.
% Bv = B�rv�rdet.
% Kp = F�rst�rkningskoefficienten = 0,75.
% Ti = Integreringstiden = 1,95.
% Td = Deriveringstiden = 0,65.
% dT = Samplingstid = 0,05.
%

% Variabler f�r anslutning initieras:
%port = '/dev/tty.usbmodem1421';    % Com-port att ansluta till (MAC h�ger).
port = '/dev/tty.usbmodem1411';     % Com-port att ansluta till (MAC v�nster).
%port = 'COM3';                     % Com-port att ansluta till (PC).
baud = 115200;                      % Baudrate.
term = 'LF';                        % Terminator-tecken.

% Variabler f�r plott initieras.
time(N) = 0;      % Tid (x-axel).
d(N) = 0.0;       % Distansv�rde (y-axel).
e(N) = 0.0;       % Felv�rde (y-axel).
u(N) = 0.0;       % Styrv�rde (y-axel).
updateTime = 1;   % Online-plottens uppdateringsfrekvens (sek).

pauseTime = 1;    % Kr�ver initiering med v�rde!

% Kontrollerar, st�nger och raderar eventuella anslutningar. 
if(~isempty(instrfind))
    fclose(instrfind);
    delete(instrfind);
end

% Uppr�ttar anslutning med Arduino.
arduino = serial(port, 'BaudRate', baud, 'Terminator', term);
fopen(arduino);
pause(0.5);             % N�dv�ndig paus!

% Rensar eventuell data i input-buffert.
flushinput(arduino);
pause(0.2);             % N�dv�ndig paus!

% Handskakning med Arduino.
fprintf(arduino, '%s\n', 's');            % S�nder startbit.
while(~arduino.BytesAvailable)            % Inv�ntar svarsbit.
    pause(0.1);
end
handshakeCode = fscanf(arduino, '%d');    % Avl�ser svarsbit.
if(handshakeCode == 1111)                 % Kontrollerar svarsbit.
    disp('Anslutning uppr�ttad.');
else
    disp('Anslutning misslyckades - k�rning avslutas.');
    return;
end

% S�nder variabler f�r reglering.
disp('Skickar data f�r reglering');
dataToSend = ['r' num2str(Bv) ';' num2str(Kp) ';' num2str(Ti) ';' num2str(Td) ';' num2str(dT) ';'];
fprintf(arduino, '%s\n', dataToSend);

pause(1);

% Programloop.
for i=1:N
    % Startar timer.
    startTime = tic;
    
    % Kontrollerar online-plottens uppdateringsfrekvens.
    if (pauseTime < 0)
        disp('Online-plottens uppdateringsfrekvens �r f�r h�g!');
        return
    end            
    
    % Beg�r r�data fr�n Arduino f�r online-plot.
    fprintf(arduino, 'd\n'); 
    while(~arduino.BytesAvailable)        % Inv�ntar data.
        pause(0.1);
    end           
    
    % Tar emot r�data fr�n Arduino.
    rawPlotData = fscanf(arduino, '%d %d %d ');
   
    % Extraherar faktisk data fr�n r�data.
    d(i) = rawPlotData(1);      % Avst�nd.
    e(i) = rawPlotData(2);      % Reglerfel.
    u(i) = rawPlotData(3);      % Styrsignal.
    
    % Uppdaterar tidsaxel.
    time(i) = i;
    
    % Ritar online-plot.
    plot(time,d,'k-', time,e,'b:', time,u,'r--');
    xlabel('tid (sek)');
    legend('avst�nd', 'reglerfel', 'styrsignal');

    % Ber�knar f�rfluten tid.
    elapsedTime = toc(startTime);
    pauseTime = (updateTime - elapsedTime);
    
    % Paus som s�kerst�llen online-plottens uppdateringsfrekvens.
    pause(pauseTime);
    
end

end
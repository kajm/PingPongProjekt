function[] = PingPong(N, Bv, Kp, Ti, Td, dT)
%
% Funktionen möjliggör online-plot av en PID-reglering
% via Arduino Due samt justering av regleringsvariabler.
%
% Skapat den: 5 april 2015.
% Skapat av: Andreas Bjärntoft & Karl Mösenbacher.
%
% VARIABLER FÖR REGLERING:
% N  = Antal samplingar.
% Bv = Börvärdet.
% Kp = Förstärkningskoefficienten = 0,75.
% Ti = Integreringstiden = 1,95.
% Td = Deriveringstiden = 0,65.
% dT = Samplingstid = 0,05.
%

% Variabler för anslutning initieras:
%port = '/dev/tty.usbmodem1421';    % Com-port att ansluta till (MAC höger).
port = '/dev/tty.usbmodem1411';     % Com-port att ansluta till (MAC vänster).
%port = 'COM3';                     % Com-port att ansluta till (PC).
baud = 115200;                      % Baudrate.
term = 'LF';                        % Terminator-tecken.

% Variabler för plott initieras.
time(N) = 0;      % Tid (x-axel).
d(N) = 0.0;       % Distansvärde (y-axel).
e(N) = 0.0;       % Felvärde (y-axel).
u(N) = 0.0;       % Styrvärde (y-axel).
updateTime = 1;   % Online-plottens uppdateringsfrekvens (sek).

pauseTime = 1;    % Kräver initiering med värde!

% Kontrollerar, stänger och raderar eventuella anslutningar. 
if(~isempty(instrfind))
    fclose(instrfind);
    delete(instrfind);
end

% Upprättar anslutning med Arduino.
arduino = serial(port, 'BaudRate', baud, 'Terminator', term);
fopen(arduino);
pause(0.5);             % Nödvändig paus!

% Rensar eventuell data i input-buffert.
flushinput(arduino);
pause(0.2);             % Nödvändig paus!

% Handskakning med Arduino.
fprintf(arduino, '%s\n', 's');            % Sänder startbit.
while(~arduino.BytesAvailable)            % Inväntar svarsbit.
    pause(0.1);
end
handshakeCode = fscanf(arduino, '%d');    % Avläser svarsbit.
if(handshakeCode == 1111)                 % Kontrollerar svarsbit.
    disp('Anslutning upprättad.');
else
    disp('Anslutning misslyckades - körning avslutas.');
    return;
end

% Sänder variabler för reglering.
disp('Skickar data för reglering');
dataToSend = ['r' num2str(Bv) ';' num2str(Kp) ';' num2str(Ti) ';' num2str(Td) ';' num2str(dT) ';'];
fprintf(arduino, '%s\n', dataToSend);

pause(1);

% Programloop.
for i=1:N
    % Startar timer.
    startTime = tic;
    
    % Kontrollerar online-plottens uppdateringsfrekvens.
    if (pauseTime < 0)
        disp('Online-plottens uppdateringsfrekvens är för hög!');
        return
    end            
    
    % Begär rådata från Arduino för online-plot.
    fprintf(arduino, 'd\n'); 
    while(~arduino.BytesAvailable)        % Inväntar data.
        pause(0.1);
    end           
    
    % Tar emot rådata från Arduino.
    rawPlotData = fscanf(arduino, '%d %d %d ');
   
    % Extraherar faktisk data från rådata.
    d(i) = rawPlotData(1);      % Avstånd.
    e(i) = rawPlotData(2);      % Reglerfel.
    u(i) = rawPlotData(3);      % Styrsignal.
    
    % Uppdaterar tidsaxel.
    time(i) = i;
    
    % Ritar online-plot.
    plot(time,d,'k-', time,e,'b:', time,u,'r--');
    xlabel('tid (sek)');
    legend('avstånd', 'reglerfel', 'styrsignal');

    % Beräknar förfluten tid.
    elapsedTime = toc(startTime);
    pauseTime = (updateTime - elapsedTime);
    
    % Paus som säkerställen online-plottens uppdateringsfrekvens.
    pause(pauseTime);
    
end

end
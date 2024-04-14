% Paramètres
Fs = 1000; % Fréquence d'échantillonnage
Fp = 1000; % Fréquence porteuse
df = 100; % Déplacement de fréquence
T = 0.01; % Durée d'un bit

% Signal modulant
bits = [1 0 1 1 0]; % Suite de bits(exemple!!)
t_mod = linspace(0, T*length(bits), length(bits)*Fs); % Temps du signal modulant par bit donc (Ts*5 = 5ms, divisé en 5000 partie donc dimension vecteur: 5000)
modulant = repelem(bits, Fs); % Signal modulant (repelem Replicate elements of an array) 
% chaque élement de bits sera répété Fs*Ts*1000 fois

% Signal modulé
t_modulated = linspace(0, T, Fs); % Temps du signal modulé
modulated = []; % Initialisation du signal modulé

% Modulation FSK
for i=1:length(bits)
    if bits(i) == 0
        modulated = [modulated, sin(2*pi*(Fp-df)*t_modulated)];
    else
        modulated = [modulated, sin(2*pi*(Fp+df)*t_modulated)];
    end
end

%Affichage des signaux
figure;
subplot(3,1,1);
plot(t_mod, modulant);
title('Signal modulant');
xlabel('Temps (s)');
ylabel('Amplitude');
subplot(3,1,2);
plot(t_mod, modulated);
title('Signal modulé');
xlabel('Temps (s)');
ylabel('Amplitude');
% subplot(3,1,3);
% plot(t_modulated, modulant.*modulated);
% title('Signal démodulé');
% xlabel('Temps (s)');
% ylabel('Amplitude');
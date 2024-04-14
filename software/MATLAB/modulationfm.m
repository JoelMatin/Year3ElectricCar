
% Paramètres signal FSK
Fs = 1000; % Fréquence d'échantillonnage
Fp = 1000; % Fréquence porteuse
df = 100; % Delta de fréquence
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



% Définir les paramètres du filtre 1
f_centrale1 = 1100; % Hz
f_utile1 = f_centrale1 * 0.015; % Hz
f_cut1 = f_centrale1 * 0.035; % Hz


% Définir les paramètres du filtre 2
f_centrale2 = 900; % Hz
f_utile2 = f_centrale2 * 0.015; % Hz
f_cut2 = f_centrale2 * 0.035; % Hz

% Calcul des coefficients du filtre1
Wn1 = [f_utile1 f_cut1] / (f_centrale1 / 2);
[b,a] = butter(4,Wn1,'bandpass');
%où 4 est l4ordre du filtre,
% Wn est un vecteur contenant les fréquences de coupure normalisées

% Calcul des coefficients du filtre2
Wn2 = [f_utile2 f_cut2] / (f_centrale2 / 2);
[d,c] = butter(4,Wn2,'bandpass');


%signal bruité(simulation)
signal_bruite = modulated + (rand(1,5000)/2)-0.25;

% Filtrer le signal d’entrée avec le filtre numérique 
signal_filtre1 = filter(b,a,signal_bruite); % signal de sortie
signal_filtre2 = filter(d,c,signal_bruite); % signal de sortie

% Transformation du signal filtré en signal binaire
%
%

%Affichage des signaux
figure;
subplot(3,1,1);
plot(t_mod, modulant);
title('Signal modulant');
xlabel('Temps (s)');
ylabel('Amplitude');
% subplot(3,1,1);
% plot(t_mod, modulated);
% title('Signal modulé');
% xlabel('Temps (s)');
% ylabel('Amplitude');
subplot(3,1,2);
plot(t_mod,signal_bruite);
title('Signal bruite');
xlabel('Temps (s)');
ylabel('Amplitude');
subplot(3,1,3);
plot(t_mod, signal_filtre2);
title('Signal filtré');
xlabel('Temps (s)');
ylabel('Amplitude');
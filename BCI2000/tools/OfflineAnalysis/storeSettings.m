function storeSettings(settings)
matFile = which('settings.mat');
save(matFile, 'settings');

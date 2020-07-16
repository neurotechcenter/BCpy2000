function settings = exportPlotsToPDF(settings, figHandles, domain)
if isfield(settings, 'exportFileDir') && ~isempty(settings.exportFileDir)
  [fn path] = uiputfile('*.pdf', 'Export to PDF', ...
    settings.exportFileDir);
else
  [fn path] = uiputfile('*.pdf', 'Export to PDF');
end

if fn ~= 0
  settings.exportFileDir = fullfile(path, '.', filesep);

  [tmp fn ext versn] = fileparts(fn);
  numErrFiles = 0;
  numFiles = 0;
  cancel = 0;
  if isfield(figHandles, 'r2') && ~isempty(figHandles.r2) 
    tmpFn = [fn ' - Feature' ext versn];
    tmpFn = fullfile(path, tmpFn);
    [writeFile cancel] = checkFileWrite(tmpFn);
    if writeFile
      try
        print(figHandles.r2, '-dpdf', tmpFn);
        numFiles = numFiles + 1;
      catch
        numErrFiles = numErrFiles + 1;
      end
    end
  end

  if ~cancel && isfield(figHandles, 'chans') && ~isempty(figHandles.chans)
    if strcmp(domain, 'time') 
      tmpFn = [fn ' - Waveforms' ext versn];
    else
      tmpFn = [fn ' - Spectra' ext versn];
    end
    tmpFn = fullfile(path, tmpFn);
    [writeFile cancel] = checkFileWrite(tmpFn);
    if writeFile
      try
        print(figHandles.chans, '-dpdf', tmpFn);
        numFiles = numFiles + 1;
      catch
        numErrFiles = numErrFiles + 1;
      end
    end
  end

  if ~cancel && isfield(figHandles, 'topos') && ~isempty(figHandles.topos)
    tmpFn = [fn ' - Topos' ext versn];
    tmpFn = fullfile(path, tmpFn);
    [writeFile cancel] = checkFileWrite(tmpFn);
    if writeFile
      try
        print(figHandles.topos, '-dpdf', tmpFn);
        numFiles = numFiles + 1;
      catch
        numErrFiles = numErrFiles + 1;
      end
    end
  end

  if ~cancel
    if numFiles == 1;
      fileTxt = 'file has';
    else
      fileTxt = 'files have';
    end
    msg = sprintf(['%d ' fileTxt ' been saved successfully.'], numFiles);
    if numErrFiles > 0
      if numErrFiles == 1
        msg = [msg '  1 file was not written because the file was open or the corresponding figure was closed.'];
      else
        msg = [msg sprintf('  %d files were not written because the files were open or the corresponding figures were closed.', ...
          numErrFiles)];
      end
    end
    uiwait(msgbox(msg));
  end
end

function [writeFile, cancel] = checkFileWrite(fn)
cancel = 0;
writeFile = 1;

if exist(fn, 'file')
  quest = ['The file ' fn ' already exists, do you want to overwrite?'];
  if exist(fn, 'file')
    switch questdlg(quest, 'File Exists', 'Yes', 'No', 'Cancel', 'No')
    case 'No'
      writeFile = 0;
    case 'Cancel'
      cancel = 1;
      writeFile = 0;
      return;
    end
  end
end


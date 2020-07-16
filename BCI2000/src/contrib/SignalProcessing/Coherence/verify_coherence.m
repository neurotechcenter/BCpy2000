function c = verify_coherence( filename, maxBlocks );

if( nargin < 2 )
    maxBlocks = 0;
end

fprintf(1,'loading file ...');
[signal, ~, parameters] = load_bcidat( filename );
done;

SamplingRate = parameters.SamplingRate.NumericValue;
SampleBlockSize = parameters.SampleBlockSize.NumericValue;
CohBufferLength = parameters.CohBufferLength.NumericValue;
CohWindowLength = parameters.CohWindowLength.NumericValue * 1e-3;
CohWindowOverlap = parameters.CohWindowOverlap.NumericValue * 1e-3;
CohFrequency = parameters.CohFrequencies.NumericValue;

if( maxBlocks > 0 )
    maxSamples = floor( maxBlocks * SampleBlockSize );
    if( maxSamples < size(signal,1) )
      signal = signal( 1:maxSamples, : );
    end
end

channel_set = [];
for( i = 1:length( parameters.TransmitChList.Value ) )
    name = parameters.TransmitChList.Value{i};
    for( j = 1:length( parameters.ChannelNames.Value ) )
        if( strcmp( name, parameters.ChannelNames.Value{j} ) )
            channel_set = [channel_set j];
        end
    end
end

signal = double( signal( :, channel_set ) );
spatial_filter = parameters.SpatialFilter.NumericValue.';
signal = signal * spatial_filter;
for( i = 1:size( signal, 2 ) )
    signal( :, i ) = signal( :, i ) - mean( signal( :, i ) );
end

fprintf(1,'Calling mscohere ...');
nfft = floor( CohWindowLength * SamplingRate );
hamm = hamming( nfft );
noverlap = floor( CohWindowOverlap * SamplingRate / 2 );
buf = floor( CohBufferLength * SamplingRate );

idx = 1:buf;
[~, F] = mscohere( signal(idx, 1), signal(idx, 2), hamming(nfft), noverlap, nfft, SamplingRate );
idx1 = find( F <= ceil( CohFrequency ) );
idx2 = find( F >= floor( CohFrequency ) );
fidx = intersect(idx1, idx2);
assert( length(fidx) == 1 );

coherence = zeros( floor( (size(signal,1)-buf)/SampleBlockSize ), length(F) );
i = 1;
while( idx(end) <= size( signal, 1 ) )
  coherence( i, : ) = mscohere( signal(idx, 1), signal(idx, 2), hamm, noverlap, nfft, SamplingRate );
  idx = idx + SampleBlockSize;
  if(mod(i,100)==1)
      fprintf(1,'\n');
  end
  fprintf(1,'.');
  i = i + 1;
end
done;

bci2000path -AddToSystemPath tools/cmdline
fprintf(1,'Running BCI2000 filter chain ...');
chain_result = bci2000chain( filename,'TransmissionFilter|SpatialFilter|CoherenceFilter');
done;
idx = floor(buf/SampleBlockSize)+1:floor(buf/SampleBlockSize)+size(coherence,1);
c = [chain_result.Signal(idx,:) coherence(:,fidx)];
e = sqrt( sum( ( c(:,1)-c(:,2) ).^2 ) / size(c,1) );
cor = corr(c);
cor = cor(1,2);

figure;
plot(c);
title(['Coherence comparison (RMS error: ' num2str(e) ', corr: ' num2str(cor) ')']);
legend('CoherenceFilter','MSCohere');


function done
fprintf(1,' done.\n');

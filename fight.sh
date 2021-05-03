WHITE=0
BLACK=0

for((batch=0;batch<10;batch++)); do
	
	MATCH_COUNT=5
	
	for((match=0;match < MATCH_COUNT; match++)); do
		./judge main main 2> log/log${match} > out/out${match} &
	done
	
	for((match=0;match < MATCH_COUNT; match++)); do
		wait
	done
		
	for((match=0;match < MATCH_COUNT; match++)); do
		read RES < out/out${match}	
		WHITE=$((WHITE+1-RES))
		BLACK=$((BLACK+0+RES))
	done
	
	echo "white = ${WHITE} black = ${BLACK}"
done

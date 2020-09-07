# credit: https://endler.dev/2017/makefiles/

# target: [dependencies]
# 	<shell command to execute>
# 	<shell command to execute>
# 	...

dress: trousers shoes jacket
	@echo "All done. Let's go outside!" >> ~/dressed.txt

jacket: pullover
	@echo "Putting on jacket." >> ~/dressed.txt

pullover: shirt
	@echo "Putting on pullover." >> ~/dressed.txt

shirt:
	@echo "Putting on shirt." >> ~/dressed.txt

trousers: underpants
	@echo "Putting on trousers." >> ~/dressed.txt

underpants:
	@echo "Putting on underpants." >> ~/dressed.txt

shoes: socks
	@echo "Putting on shoes." >> ~/dressed.txt

socks: pullover
	@echo "Putting on socks." >> ~/dressed.txt

.PHONY: clean
clean: 
	rm ~/dressed.txt

zip: 
	echo "This is how I get dressed in the morning!" >> ~/README
	zip ~/dress_dyi.zip ~/README ~/dressed.txt

.PHONY: clean All

All:
	@echo "----------Building project:[ ALNS_OOP - Release ]----------"
	@cd "ALNS_OOP" && "$(MAKE)" -f  "ALNS_OOP.mk"
clean:
	@echo "----------Cleaning project:[ ALNS_OOP - Release ]----------"
	@cd "ALNS_OOP" && "$(MAKE)" -f  "ALNS_OOP.mk" clean

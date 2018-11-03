all: run

cmp:
	@vcs -sverilog -debug_pp +memcbk -f filelist.f -top tb_top

run: cmp
	@./simv

dve:
	@dve -vpd wave.vpd &

clean:
	@rm -rf simv csrc simv.daidir ucli.key DVEfiles wave.vpd

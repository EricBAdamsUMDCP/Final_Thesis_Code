#!/bin/bash



for RN in 1 2 3 326776 326822 326883 326943 327078 327211 327327 327464 327560
do
	mkdir -p /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/PNG/LOOSE/Normal /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/PNG/LOOSE/WIDE /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/PNG/LOOSE/NARROW 
	mkdir -p /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/ROOT/LOOSE/Normal /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/ROOT/LOOSE/WIDE /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/ROOT/LOOSE/NARROW 

	mkdir -p /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/PNG/NOMINAL/Normal /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/PNG/NOMINAL/WIDE /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/PNG/NOMINAL/NARROW 
	mkdir -p /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/ROOT/NOMINAL/Normal /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/ROOT/NOMINAL/WIDE /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/ROOT/NOMINAL/NARROW 

	mkdir -p /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/PNG/TIGHT/Normal /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/PNG/TIGHT/WIDE /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/PNG/TIGHT/NARROW 
	mkdir -p /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/ROOT/TIGHT/Normal /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/ROOT/TIGHT/WIDE /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/ROOT/TIGHT/NARROW 

	mkdir -p /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/PNG/LOOSE/Normal /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/PNG/LOOSE/WIDE /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/PNG/LOOSE/NARROW 
	mkdir -p /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/ROOT/LOOSE/Normal /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/ROOT/LOOSE/WIDE /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/ROOT/LOOSE/NARROW 

	mkdir -p /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/PNG/NOMINAL/Normal /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/PNG/NOMINAL/WIDE /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/PNG/NOMINAL/NARROW 
	mkdir -p /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/ROOT/NOMINAL/Normal /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/ROOT/NOMINAL/WIDE /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/ROOT/NOMINAL/NARROW 

	mkdir -p /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/PNG/TIGHT/Normal /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/PNG/TIGHT/WIDE /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/PNG/TIGHT/NARROW 
	mkdir -p /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/ROOT/TIGHT/Normal /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/ROOT/TIGHT/WIDE /data2/users/ebadams/Eric_SP_Data_Sets/HF/"$RN"/ROOT/TIGHT/NARROW 


	mkdir -p /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/PNG/LOOSE/Normal /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/PNG/LOOSE/WIDE /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/PNG/LOOSE/NARROW 
	mkdir -p /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/ROOT/LOOSE/Normal /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/ROOT/LOOSE/WIDE /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/ROOT/LOOSE/NARROW 

	mkdir -p /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/PNG/NOMINAL/Normal /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/PNG/NOMINAL/WIDE /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/PNG/NOMINAL/NARROW 
	mkdir -p /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/ROOT/NOMINAL/Normal /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/ROOT/NOMINAL/WIDE /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/ROOT/NOMINAL/NARROW 

	mkdir -p /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/PNG/TIGHT/Normal /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/PNG/TIGHT/WIDE /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/PNG/TIGHT/NARROW 
	mkdir -p /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/ROOT/TIGHT/Normal /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/ROOT/TIGHT/WIDE /data2/users/ebadams/Eric_RPD_RXN_Data_Sets/"$RN"/ROOT/TIGHT/NARROW


	mkdir -p /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/PNG/LOOSE/Normal /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/PNG/LOOSE/WIDE /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/PNG/LOOSE/NARROW 
	mkdir -p /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/ROOT/LOOSE/Normal /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/ROOT/LOOSE/WIDE /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/ROOT/LOOSE/NARROW 

	mkdir -p /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/PNG/NOMINAL/Normal /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/PNG/NOMINAL/WIDE /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/PNG/NOMINAL/NARROW 
	mkdir -p /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/ROOT/NOMINAL/Normal /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/ROOT/NOMINAL/WIDE /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/ROOT/NOMINAL/NARROW 

	mkdir -p /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/PNG/TIGHT/Normal /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/PNG/TIGHT/WIDE /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/PNG/TIGHT/NARROW 
	mkdir -p /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/ROOT/TIGHT/Normal /data2/users/ebadams/Eric_HF_RXN_Data_Sets/"$RN"/ROOT/TIGHT/WIDE /data2/users/ebadams/Eric_SP_Data_Sets/RPD/"$RN"/ROOT/TIGHT/NARROW 
done


echo All Directories Made
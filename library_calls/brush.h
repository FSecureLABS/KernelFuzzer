/* Brush Functions START */


VOID BH_GetSysColorBrush() {

	HBRUSH result_BH_GetSysColorBrush;
	int tempInt_BH_GetSysColorBrush;

	char vid[16];
	sprintf(vid, "%d%d", get_time_in_ms(), rand() % 1024);
	logger("HBRUSH result_BH_GetSysColorBrush%s;", vid);
	logger("int tempInt_BH_GetSysColorBrush%s;", vid);

	tempInt_BH_GetSysColorBrush = get_fuzzed_int32();
	logger("tempInt_BH_GetSysColorBrush%s = %d;", vid, tempInt_BH_GetSysColorBrush);

	logger("//[Library_Call]: BH_GetSysColorBrush");
	logger("result_BH_GetSysColorBrush%s = GetSysColorBrush(tempInt_BH_GetSysColorBrush%s);", vid, vid);
	result_BH_GetSysColorBrush = GetSysColorBrush(tempInt_BH_GetSysColorBrush);

	logger("put_random_HANDLE(result_BH_GetSysColorBrush%s, TEXT(\"BH_GetSysColorBrush\"));", vid);
	put_random_HANDLE(result_BH_GetSysColorBrush, TEXT("BH_GetSysColorBrush"));
}



/* Brush Functions END */
Include "dir_util.bas"
ChangeDir$(SourceDirectory$)

Dim path_join$

If OS$ = "WINDOWS" Then
	path_join$ = "\\"
Else
	path_join$ = "/"
End if

Function gen_app_settings(app_name$, icon_path$, orientation$, app_id$, java_app_path$)
	cwd$ = Dir$
	
	project_strings_xml$ = Dir$+path_join$+".."+path_join$+"android-project"+path_join$+"app"+path_join$+"src"+path_join$+"main"+path_join$+"res"+path_join$+"values"+path_join$+"strings.xml"
	
	If FileExists(project_strings_xml$) Then
		RemoveFile(project_strings_xml$)
		Print "------strings.xml deleted-------"
	End If
	
	return_val = True
	
	If Right$(Dir$, 7) <> "scripts" Then
		Print "Error: Could not find scripts folder"
		return_val = False
	End If
	
	strings_xml_base = OpenFile("values" + path_join$ + "strings.xml", TEXT_INPUT)
	If return_val And ( strings_xml_base < 0 ) Then
		Print "Error: Missing strings.xml in scripts folder"
		return_val = False
	End If
	strings_xml_out = OpenFile(project_strings_xml$, TEXT_OUTPUT)
	If return_val And ( strings_xml_out < 0 ) Then
		CloseFile(strings_xml_base)
		Print "Error: Missing strings.xml in base project folder"
		Return False
	End If
	While return_val And ( Not EOF(strings_xml_base) )
		ln$ = ReadLine$(strings_xml_base)
		ln$ = Replace(ln$, "[rcbasic_app_name]", app_name$)
		WriteLine(strings_xml_out, ln$)
	Wend
	
	CloseFile(strings_xml_base)
	CloseFile(strings_xml_out)
	
	manifest_name$ = "AndroidManifest.xml"
	manifest_out$ = Dir$+path_join$+".."+path_join$+"android-project"+path_join$+"app"+path_join$+"src"+path_join$+"main"+path_join$+manifest_name$
	
	If FileExists(manifest_out$) Then
		RemoveFile(manifest_out$)
	End If
	
	manifest_tmp_file = OpenFile(manifest_name$, TEXT_INPUT)
	If manifest_tmp_file < 0 Then
		Print "Error: Manifest template not found"
		Return False
	End If
	
	manifest_out_file = OpenFile(manifest_out$, TEXT_OUTPUT)
	If manifest_out_file < 0 Then
		Print "Error: Can't output AndroidManifest.xml"
		Return False
	End If
	
	Dim project_dir$
	Dim java_file_path$
	
	project_dir$ = Env$("PROJECT_DIR")
	
	activity_name$ = "rcbasic_" + Replace(app_name$, " ", "_")
	java_file_path$ = project_dir$ + path_join$ + activity_name$ + ".java"
	
	java_file_exists = FileExists(java_file_path$)
	
	If java_file_exists Then
		If Right$(java_app_path$, 1) <> path_join$ Then
			java_app_path$ = java_app_path$ + path_join$
		End If
		CopyFile(java_file_path$, java_app_path$ + activity_name$ + ".java")
	End If
	
	Print "Checking if EXISTS["; java_file_path$; "] --> ";java_file_exists
	
	While Not EOF(manifest_tmp_file)
		ln$ = Readline$(manifest_tmp_file)
		
		ln_out$ = ln$
		ln_out$ = Replace(ln_out$, "[rcbasic_display]", orientation$)
		
		If java_file_exists
			ln_out$ = Replace(ln_out$, "SDLActivity", app_id$ + "." + activity_name$)
		End If
		
		If orientation$ = "default" And ln_out$ <> ln$ Then
			ln_out$ = ""
		End If
		
		Writeline(manifest_out_file, ln_out$)
	Wend
	
	CloseFile(manifest_tmp_file)
	CloseFile(manifest_out_file)
	
	If FileExists(Replace(Replace("../android-project/app/src/main/res/[res_dir]/ic_launcher.png", "/", path_join$),"[res_dir]","mipmap-mdpi")) Then
		RemoveFile(Replace(Replace("../android-project/app/src/main/res/[res_dir]/ic_launcher.png", "/", path_join$),"[res_dir]","mipmap-mdpi"))
	End If
	
	If FileExists(Replace(Replace("../android-project/app/src/main/res/[res_dir]/ic_launcher.png", "/", path_join$),"[res_dir]","mipmap-hdpi")) Then
		RemoveFile(Replace(Replace("../android-project/app/src/main/res/[res_dir]/ic_launcher.png", "/", path_join$),"[res_dir]","mipmap-hdpi"))
	End If
	
	If FileExists(Replace(Replace("../android-project/app/src/main/res/[res_dir]/ic_launcher.png", "/", path_join$),"[res_dir]","mipmap-xhdpi")) Then
		RemoveFile(Replace(Replace("../android-project/app/src/main/res/[res_dir]/ic_launcher.png", "/", path_join$),"[res_dir]","mipmap-xhdpi"))
	End If
	
	If FileExists(Replace(Replace("../android-project/app/src/main/res/[res_dir]/ic_launcher.png", "/", path_join$),"[res_dir]","mipmap-xxhdpi")) Then
		RemoveFile(Replace(Replace("../android-project/app/src/main/res/[res_dir]/ic_launcher.png", "/", path_join$),"[res_dir]","mipmap-xxhdpi"))
	End If
	
	If FileExists(Replace(Replace("../android-project/app/src/main/res/[res_dir]/ic_launcher.png", "/", path_join$),"[res_dir]","mipmap-xxxhdpi")) Then
		RemoveFile(Replace(Replace("../android-project/app/src/main/res/[res_dir]/ic_launcher.png", "/", path_join$),"[res_dir]","mipmap-xxxhdpi"))
	End If
	
	'Do stuff with the icons
	If FileExists(icon_path$) Then
		image_magic_path$ = Dir$ + path_join$ + ".." + path_join$ + "ImageMagick"
		Print "SET to PATH:";image_magic_path$
		SetEnv("PATH", Env("PATH") + ":" + image_magic_path$)
		changedir(image_magic_path$)
		Dim resize_cmd$
		If UCase$(OS$) = "LINUX" Then
			resize_cmd$ = Replace("magick convert \q[icon_path]\q -resize [size] ../android-project/app/src/main/res/[res_dir]/ic_launcher.png", "/", path_join$)
		Else
			'SetEnv("PATH", Env$("PATH")+";"+image_magic_path$,1)
			resize_cmd$ = Replace("convert \q[icon_path]\q -resize [size] ../android-project/app/src/main/res/[res_dir]/ic_launcher.png", "/", path_join$)
		End If
		resize_cmd$ = Replace(resize_cmd$, "[icon_path]", icon_path$)
		CURRENT_PATH$ = Env("PATH")
		'SetEnv("PATH", image_magic_path$ + ":" + Env("PATH"), 1)
		print "\n\nRES_CMD::";Replace(Replace(resize_cmd$,"[size]","48x48"),"[res_dir]","mipmap-mdpi"); "\nin\n"; Dir$;"\n\n"
		print "MGK VERSION:"; System("magick --version")
		print "SRC DIR = "; dir$
		print "\n\n"
		
		Print "error code = ";System(Replace(Replace(resize_cmd$,"[size]","48x48"),"[res_dir]","mipmap-mdpi"))
		Print "error code = ";System(Replace(Replace(resize_cmd$,"[size]","72x72"),"[res_dir]","mipmap-hdpi"))
		Print "error code = ";System(Replace(Replace(resize_cmd$,"[size]","96x96"),"[res_dir]","mipmap-xhdpi"))
		Print "error code = ";System(Replace(Replace(resize_cmd$,"[size]","144x144"),"[res_dir]","mipmap-xxhdpi"))
		Print "error code = ";System(Replace(Replace(resize_cmd$,"[size]","192x192"),"[res_dir]","mipmap-xxxhdpi"))
		changedir(cwd$)
		SetEnv("PATH", CURRENT_PATH$)
	Else
		image_magic_path$ = Dir$ + path_join$ + "ImageMagick"
		copy_location$ = Replace("../android-project/app/src/main/res/[res_dir]/ic_launcher.png", "/", path_join$)
		CopyFile("mipmap-mdpi"+path_join$+"ic_launcher.png", Replace(copy_location$,"[res_dir]","mipmap-mdpi"))
		CopyFile("mipmap-hdpi"+path_join$+"ic_launcher.png", Replace(copy_location$,"[res_dir]","mipmap-hdpi"))
		CopyFile("mipmap-xhdpi"+path_join$+"ic_launcher.png", Replace(copy_location$,"[res_dir]","mipmap-xhdpi"))
		CopyFile("mipmap-xxhdpi"+path_join$+"ic_launcher.png", Replace(copy_location$,"[res_dir]","mipmap-xxhdpi"))
		CopyFile("mipmap-xxxhdpi"+path_join$+"ic_launcher.png", Replace(copy_location$,"[res_dir]","mipmap-xxxhdpi"))
	End If
	

	Return return_val
End Function

Function gen_build_gradle(api_version, min_api_version, app_id$, release, key_store$, key_store_pass$, alias$, alias_pass$)
	cwd$ = Dir$
	
	print "RELEASE FLAG = ";release
	
	sign_config$ = ""
	sign_config_file = OpenFile("sign_config_template", TEXT_INPUT)
	If sign_config_file < 0 Then
		Return False
	End If
	
	While Not EOF(sign_config_file)
		sign_config$ = sign_config + ReadLine$(sign_config_file) + "\n"
	Wend
	
	CloseFile(sign_config_file)
	
	sign_config$ = Replace(sign_config$, "rc_keystore.jks", key_store$)
	sign_config$ = Replace(sign_config$, "rc_keystore_password", key_store_pass$)
	sign_config$ = Replace(sign_config$, "rc_alias_name", alias$)
	sign_config$ = Replace(sign_config$, "rc_alias_password", alias_pass$)
	
	'ChangeDir("scripts")
	If FileExists(Dir$+path_join$+".."+path_join$+"android-project"+path_join$+"app"+path_join$+"build.gradle") Then
		RemoveFile(Dir$+path_join$+".."+path_join$+"android-project"+path_join$+"app"+path_join$+"build.gradle")
		Print "------Build Gradle deleted-------"
	End If
	return_val = True
	If Right$(Dir$, 7) <> "scripts" Then
		Print "Error: Could not find scripts folder"
		return_val = False
	End If
	build_gradle_base = OpenFile("build_base.gradle", TEXT_INPUT)
	If return_val And ( build_gradle_base < 0 ) Then
		Print "Error: Missing build_base.gradle in scripts folder"
		return_val = False
	End If
	build_gradle_out = OpenFile(Dir$+path_join$+".."+path_join$+"android-project"+path_join$+"app"+path_join$+"build.gradle", TEXT_OUTPUT)
	If return_val And ( build_gradle_out < 0 ) Then
		CloseFile(build_gradle_base)
		Print "Error: Missing build_base.gradle in scripts folder"
		Return False
	End If
	While return_val And ( Not EOF(build_gradle_base) )
		ln$ = ReadLine$(build_gradle_base)
		
		ln$ = Replace(ln$, "RCBASIC_APP_ID", app_id$)
		ln$ = Replace(ln$, "RCBASIC_API_VERSION", Str$(api_version))
		ln$ = Replace(ln$, "RCBASIC_MIN_API_VERSION", Str$(min_api_version))
		If release Then
			ln$ = Replace(ln$, "//rc_signingConfig_block", sign_config$)
			ln$ = Replace(ln$, "//rc_signConfig_release", "signingConfig signingConfigs.release")
		End If
		
		WriteLine(build_gradle_out, ln$)
	Wend
	
	CloseFile(build_gradle_base)
	CloseFile(build_gradle_out)
	
	'ChangeDir(cwd$)
	
	Return return_val
End Function

Function gen_jv_src()
	current_dir$ = Dir$
	If Right$(current_dir$, 1) = path_join$ Then
		current_dir$ = Left$(current_dir$, Len(current_dir$)-1)
	End If
	ChangeDir(current_dir$+path_join$+".."+path_join$+"android-project"+path_join$+"app"+path_join$+"src"+path_join$+"main"+path_join$+"java")
	
	Dim purge_list$[99]
	Dim purge_count
	
	ArrayFill(purge_list$, "")
	purge_count = 0
	
	item$ = Trim(DirFirst())
	While item <> ""
		If item <> "org" And Left(item,1) <> "." Then
			purge_list$[purge_count] = item$
			purge_count = purge_count + 1
		End If
		item = Trim(DirNext())
	Wend
	
	For i = 0 To purge_count-1
		Print "DELETE: "; purge_list$[i]
		System("rm -rf " + purge_list$[i])
		If DirExists(purge_list$[i]) Then
			System("rmdir " + purge_list$[i])
		End If
	Next
	
	ArrayFill(purge_list$, "")
	purge_count = 0
	
	ChangeDir("org")
	item$ = Trim(DirFirst())
	While item <> ""
		If item <> "libsdl" And Left(item,1) <> "." Then
			purge_list$[purge_count] = item$
			purge_count = purge_count + 1
		End If
		item = Trim(DirNext())
	Wend
	
	For i = 0 To purge_count-1
		Print "DELETE: "; purge_list$[i]
		System("rm -rf " + purge_list$[i])
		If DirExists(purge_list$[i]) Then
			System("rmdir " + purge_list$[i])
		End If
	Next
	
	ChangeDir(current_dir$)
	
	Return 0
	
End Function


'Clear old project files
gen_jv_src()


If NumCommands() > 2 Then
	app_name$ = ""
	orientation$ = ""
	
	icon_path$ = ""
	
	api_version = 0
	min_api_version = 0
	app_id$ = ""
	
	release = false
	
	key_store$ = ""
	key_store_pass$ = ""
	alias$ = ""
	alias_pass$ = ""
	
	For i = 1 to NumCommands()-2
		Select Case Trim(Command(i))
		Case "--APP_NAME"
			app_name$ = Trim(Command(i+1))
		Case "--ORIENTATION"
			orientation$ = Trim(Command(i+1))
		Case "--ICON"
			icon_path$ = Trim(Command(i+1))
		Case "--API_VERSION"
			api_version = Int(Val(Command(i+1)))
		Case "--MIN_API_VERSION"
			min_api_version = Int(Val(Command(i+1)))
		Case "--APP_ID"
			app_id$ = Trim(Command(i+1))
		Case "--RELEASE"
			release = true
		Case "--KEY_STORE"
			key_store$ = Trim(Command(i+1))
		Case "--KEY_STORE_PASS"
			key_store_pass$ = Trim(Command(i+1))
		Case "--ALIAS"
			alias$ = Trim(Command(i+1))
		Case "--ALIAS_PASS"
			alias_pass$ = Trim(Command(i+1))
		End Select
	Next
	
	current_dir$ = Dir$()
	ChangeDir$("../android-project/app/src/main/java")
	adj_app_id$ = app_id$ + "."
	id_module$ = ""
	java_app_path$ = ""
	For i = 0 To Len(adj_app_id$)-1
		If Mid(adj_app_id$, i, 1) = "." Then
			If Not DirExists(id_module$) Then
				MakeDir(id_module$)
			End If
			ChangeDir(id_module$)
			id_module$ = ""
		Else
			id_module$ = id_module$ + Mid(adj_app_id$, i, 1)
		End If
	Next
	java_app_path$ = Dir$()
	ChangeDir$(current_dir$)
	
	If Trim(Command(NumCommands()-1)) = "--RELEASE" Then
		release = true
	End If
	
	If app_name$ = "" Then
		Print "Error: Missing App Name for android build"
		End
	End If
	
	If orientation$ = "" Then
		orientation= "default"
	End If
	
	If Not gen_app_settings(app_name$, icon_path$, orientation$, app_id$, java_app_path$) Then
		Print "Error: Failed to generate android settings with args: "
		Print "APP_NAME=";app_name$
		Print "ICON=";icon_path$
		End
	End If
	
	If api_version=0 Or min_api_version=0 Or app_id$="" Then
		Print "Error: Missing Args for gen_build_gradle"
		Print "API_VERSION=";api_version
		Print "MIN_API_VERSION=";min_api_version
		Print "APP_ID=";app_id$
		End
	ElseIf Not gen_build_gradle(api_version, min_api_version, app_id$, release, key_store, key_store_pass, alias, alias_pass) Then
		Print "Error: Failed to generate build.gradle with args: "
		Print "API_VERSION=";api_version
		Print "MIN_API_VERSION=";min_api_version
		Print "APP_ID=";app_id$
		Print "ORIENTATION=";orientation$
		End
	End If
	
Else
	Print "Error: Missing Args for gen_app_settings"
	End
End If

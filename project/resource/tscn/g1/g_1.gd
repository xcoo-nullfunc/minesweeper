extends Node2D

var options = JSON.new()
var json : Variant

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	'''
	var FWT = FileAccess.open("D:/__workbench/Godot/godot-cpp/_export/minesweeper/FileWriteTest.txt", FileAccess.WRITE);
	FWT.store_string("test 123")
	FWT.close()
	'''
	
	options = JSON.new()
	var json_text : String
	if(FileAccess.file_exists("user://options.json")):
		var file_i = FileAccess.open("user://options.json", FileAccess.READ)
		json_text = file_i.get_as_text()
		if(options.parse(json_text) == OK):
			json = options.data
			print("options.json prased")
		else:
			json_text = '{"MST" : {"quick_click" : false, }, "Camera" : {"move_speed" : 150, "quick_move_multi" : 3,"move_sensitive" : 1, "reverse_move" : false, }, }'
			json = options.parse(json_text)
			print("options.json prase failed")
		
		if(json is float):
			json_text = '{"MST" : {"quick_click" : false, }, "Camera" : {"move_speed" : 150, "quick_move_multi" : 3,"move_sensitive" : 1, "reverse_move" : false, }, }'
			json = options.parse(json_text)
			print("options.json prase failed")
		print(json)
		file_i.close()
	else:
		print("options.json not exists or prase failed, creating a default file...")
		json_text = '{"MST" : {"quick_click" : false, }, "Camera" : {"move_speed" : 150, "quick_move_multi" : 3,"move_sensitive" : 1, "reverse_move" : false, }, }'
		json = options.parse(json_text)
	
	$MST.quick_click = options.data.get("MST").get("quick_click", false)
	$CanvasLayer/options_bg/options/quick_click/CheckBox_quick_click.button_pressed = $MST.quick_click
	$Camera2D.speed = options.data.get("Camera", {}).get("move_speed", 150)
	$CanvasLayer/options_bg/options/move_speed/SpinBox_move_speed.value = $Camera2D.speed
	$Camera2D.quick_move_multi = options.data.get("Camera", {}).get("quick_move_multi", 3)
	$CanvasLayer/options_bg/options/qm_multi/SpinBox_qm_multi.value = $Camera2D.quick_move_multi
	$Camera2D.move_sensitive = options.data.get("Camera", {}).get("move_sensitive", 1)
	$CanvasLayer/options_bg/options/mouse_sensitive/SpinBox_mouse_sensitive.value = $Camera2D.move_sensitive
	$Camera2D.reverse_move = options.data.get("Camera", {}).get("reverse_move", false)
	$CanvasLayer/options_bg/options/reserve_mouse/CheckBox_reserve_mouse.button_pressed = $Camera2D.reverse_move
	pass # Replace with function body.

func _exit_tree() -> void:
	save_options() 

func save_options() -> void :
	var json_text : String = JSON.stringify(options.data, "\t")
	var file_o = FileAccess.open("user://options.json", FileAccess.WRITE)
	file_o.store_string(json_text)
	file_o.close()

func set_quick_click(qc : bool) -> void:
	self.options.data["MST"]["quick_click"] = qc

func set_move_speed(ms : float) -> void:
	self.options.data["Camera"]["move_speed"] = ms

func set_quick_move_multi(multi : float) -> void:
	self.options.data["Camera"]["quick_move_multi"] = multi

func set_mouse_sensitive(sensitive : float) -> void:
	self.options.data["Camera"]["move_sensitive"] = sensitive

func set_reserve_mouse(reserve : bool) -> void:
	self.options.data["Camera"]["reverse_move"] = reserve

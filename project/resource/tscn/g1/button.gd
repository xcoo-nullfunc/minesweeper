extends Button

func _process(delta: float) -> void:
	pass

func input_cancle():
	$"../../..".visible = false

func _gui_input(event: InputEvent) -> void:
	if(event is InputEventMouseButton):
		if(event.is_released()):
			input_cancle()

func _input(event: InputEvent) -> void:
	if(event is InputEventKey):
		if(event.pressed):
			if(event.keycode == KEY_ESCAPE):
				input_cancle()

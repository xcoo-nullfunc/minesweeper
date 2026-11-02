extends Button

func start_game() -> void:
	var w : int = int($"../../VFlowContainer4/SpinBox_X".value)
	var h : int = int($"../../VFlowContainer3/SpinBox_Y".value)
	var n : int = int($"../../VFlowContainer2/SpinBox_N".value)
	$"../../../../../MST".initMS(w, h, n)
	$"../../..".visible = false

func _gui_input(event: InputEvent) -> void:
	if(event is InputEventMouseButton):
		if(event.is_released()):
			start_game()

func _input(event: InputEvent) -> void:
	if(event is InputEventKey):
		if(event.pressed and not event.echo):
			if(event.keycode == KEY_ENTER and $"../../..".visible == true):
				start_game()
			elif(event.keycode == KEY_N):
				$"../../..".visible = true;

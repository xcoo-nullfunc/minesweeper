extends TextureButton



func _on_button_up() -> void:
	if($"../../../MST".g_over):
		$"../../Container/new_game/VFlowContainer/Button_start".start_game()

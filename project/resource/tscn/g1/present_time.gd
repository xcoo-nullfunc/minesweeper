extends Label

func _ready() -> void:
	self.size.x = get_viewport_rect().size.x - 30;

func _physics_process(delta: float) -> void:
	self.text = str(int($"../../MST".TimeCounter_get_time()))

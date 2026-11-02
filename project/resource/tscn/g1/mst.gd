extends MST

var TimeCounter1 = TimeCounter.new()
var g_over = false

func TimeCounter_start() -> void:
	TimeCounter1.start();
	
func TimeCounter_pause() -> void:
	TimeCounter1.pause();
	g_over = true

func TimeCounter_reset() -> void:
	TimeCounter1.reset();
	g_over = false;

func TimeCounter_get_time() -> float:
	return TimeCounter1.get_time();

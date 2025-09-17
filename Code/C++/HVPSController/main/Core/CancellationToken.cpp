#include "CancellationToken.hpp"
#include "UsageGuard.hpp"

CancellationToken::CancellationToken(bool initiallyCancelled)
: _eventGroupHandle(nullptr)
, _mutex(nullptr)
, _callbacks(){
    _eventGroupHandle  = xEventGroupCreate();
    _mutex = xSemaphoreCreateMutex();          // <-- missing
    configASSERT(_eventGroupHandle && _mutex);
    if (initiallyCancelled) {
        xEventGroupSetBits(_eventGroupHandle, _cancelledBIT);
    }
}

CancellationToken::~CancellationToken() {
    if (_eventGroupHandle) { vEventGroupDelete(_eventGroupHandle); _eventGroupHandle = nullptr; }
    if (_mutex)           { vSemaphoreDelete(_mutex);             _mutex           = nullptr; }
}

void CancellationToken::cancel() {

    xSemaphoreTake(_mutex, portMAX_DELAY);
    if (isCancellationRequested()){
		xSemaphoreGive(_mutex);
		return;
	}
	xEventGroupSetBits(_eventGroupHandle, _cancelledBIT);
	auto callbacks = _callbacks;
	xSemaphoreGive(_mutex);
	for (auto& callback : callbacks) {
		if (callback) callback();
	}
}

bool CancellationToken::isCancellationRequested() const {
    return (xEventGroupGetBits(_eventGroupHandle) & _cancelledBIT) != 0;
}

void CancellationToken::wait() const {
    UsageGuard<const CancellationToken> g(this);
    (void)xEventGroupWaitBits(
        _eventGroupHandle, _cancelledBIT,
        pdFALSE, pdTRUE,
        portMAX_DELAY
    );
}

bool CancellationToken::wait(uint32_t timeout_ms) const {
    UsageGuard<const CancellationToken> g(this);
    EventBits_t bits = xEventGroupWaitBits(
        _eventGroupHandle, _cancelledBIT,
        pdFALSE, pdTRUE,
        pdMS_TO_TICKS(timeout_ms)
    );
    return (bits & _cancelledBIT) != 0;
}

int CancellationToken::reg(std::function<void()> callback) {
    if (!callback) return -1;

    xSemaphoreTake(_mutex, portMAX_DELAY);
    if (isCancellationRequested()) {
        xSemaphoreGive(_mutex);
        callback(); // run immediately if already cancelled
        return -1;
    }
    _callbacks.push_back(std::move(callback));
    int handle = static_cast<int>(_callbacks.size()) - 1;
    xSemaphoreGive(_mutex);
    return handle;
}

void CancellationToken::deregister(int handle) {
    if (handle < 0) return;
    xSemaphoreTake(_mutex, portMAX_DELAY);
    if (handle < static_cast<int>(_callbacks.size())) {
        _callbacks[static_cast<size_t>(handle)] = nullptr;
    }
    xSemaphoreGive(_mutex);
}

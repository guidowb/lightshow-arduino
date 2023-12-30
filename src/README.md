## Protocol

Devices use MQTT with a topic prefix of `guidowb.online/` to enable the use
of public brokers. Devices further use an app specific prefix of `lightshow/`
to enable apps to subscribe to app specific topics.

The protocol structure in general is:
```
<-- guidowb.online/lightshow/spec/%optional_client { spec }
--> guidowb.online/lightshow/state/%client { state }
```

Spec (`spec`) is used to communicate desired state to clients, idempotently.
State (`state`) is used to communicate actual state to controllers/apps.

Servers are expected to monitor actual state and re-transmit desired state if
actual state is not reached within an allotted amount of time.

Clients are expected to communicate actual state periodically (every 30 seconds),
or immediately when state changes.

## Discovery

Devices respond to:
```
guidowb.online/ping
guidowb.online/lightshow/ping
guidowb.online/lightshow/ping/%c
```
with:
```
guidowb.online/lightshow/pong/%c
```

## Specs

Controllers determine when and how to communicate desired state. Typically, they should only do
so when actual state is not desired state (and only for the desired differences), but they may
choose to send complete desired state when unsure of the desired state the device has captured.

Devices are expected to attempt to reconcile any differences with actual state and report effected
changes via state updates.

## State Reporting

Devices transmit complete state (every 30 seconds on idle, and immediately after a direct ping):
```
guidowb.online/lightshow/state/%c { complete_state }
```

Devices should immediately transmit changes in state via:
```
guidowb.online/lightshow/state/%c { aspect_state }
```

Such notifications should not reset the 30 second full state transmission interval,
but may pre-empt a full state transmission if it's close to the next time to send one.

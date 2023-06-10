import React, { useState, useEffect, useRef } from 'react';
import { w3cwebsocket as W3CWebSocket } from 'websocket';

import CircularSlider from '@fseehawer/react-circular-slider';

// import from material ui
import Switch from '@mui/material/Switch';
import PowerSettingsNewIcon from '@mui/icons-material/PowerSettingsNew';

const PJU = () => {
	const websocket = useRef(null);
	const [status, setStatus] = useState(false);
	const [dimmer, setDimmer] = useState(0);

	const sentDimmer = e => {
		setDimmer(e);
		console.log(e);
		websocket.current.send(
			JSON.stringify({
				type: 'dimmer',
				dimmer: dimmer,
			})
		);
	};

	const sentStatus = e => {
		setStatus(e);
		console.log(e);
		websocket.current.send(
			JSON.stringify({
				type: 'status',
				status: status,
			})
		);
	};

	useEffect(() => {
		websocket.current = new W3CWebSocket('ws://192.168.2.1/pju/ws');
		websocket.current.onmessage = message => {
			const dataFromServer = JSON.parse(message.data);
			console.log(dataFromServer);
			if (dataFromServer.type === 'retain') {
				setDimmer(dataFromServer.dimmer);
				setStatus(dataFromServer.status);
			}

			if (dataFromServer.type === 'ack') {
				setDimmer(dataFromServer.dimmer);
				setStatus(dataFromServer.status);
			}
		};
		return () => websocket.current.close();
	}, []);

	return (
		<>
			<div className="py-5" />
			<div className="mx-auto h-[75%] w-[350px] rounded-2xl bg-primary text-center">
				<h1 className="mb-14 pt-10 text-2xl font-bold tracking-widest text-white">CONTROL</h1>
				<CircularSlider
					width={200}
					label="DIM"
					min={0}
					max={100}
					dataIndex={0}
					appendToValue="%"
					labelColor="#ffffff"
					knobColor="#3479ff"
					knobSize={50}
					progressColorFrom="#3479ff"
					progressColorTo="#25ced5"
					progressSize={15}
					trackColor="#eeeeee"
					trackSize={13}
					onChange={e => sentDimmer(e)}
				/>
				{/* <DragIcon x="22" y="22" width="28px" height="28px" /> */}
				{/* </CircularSlider> */}

				<div className="mx-auto mt-10 h-20 w-5/6 rounded-lg bg-[#2F2F5C] px-5">
					<div className="flex h-full items-center justify-between">
						<div className="flex items-center">
							<div className="relative h-fit w-fit">
								<div className={`absolute -inset-0 z-10 rounded-sm bg-bluish blur-sm`} />
								<div
									className={`relative z-20 flex h-14 w-14 items-center justify-center rounded-lg bg-bluish`}>
									<PowerSettingsNewIcon fontSize="large" />
								</div>
							</div>
							<h2 className="ml-5 text-lg font-bold tracking-wider text-white">Power</h2>
						</div>
						<Switch checked={status} onChange={e => sentStatus(e.target.checked)} />
					</div>
				</div>
			</div>
		</>
	);
};

export default PJU;
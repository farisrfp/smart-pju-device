import React, { useState, useEffect, useRef } from 'react';
import { w3cwebsocket as W3CWebSocket } from 'websocket';

import Card from '../components/Card';
import DateTime from '../components/DateTime';
import GlowingText from '../components/utils/GlowingText';

import volIcon from '../asset/image 10.png';
import curIcon from '../asset/image 9.png';
import tempIcon from '../asset/image 7.png';
import lightIcon from '../asset/image 8.png';

import xcamp from '../asset/Group 20.png';

const Home: React.FC = () => {
	const websocket = useRef<W3CWebSocket>();
	const [vol, setVol] = useState(220);
	const [cur, setCur] = useState(800);
	const [temp, setTemp] = useState(25);
	const [light, setLight] = useState(40);
	const [rtc, setRtc] = useState(1685435353);

	useEffect(() => {
		websocket.current = new W3CWebSocket('ws://192.168.2.1/ws');
		// eslint-disable-next-line arrow-parens
		websocket.current.onmessage = message => {
			const dataFromServer = JSON.parse(message.data.toString());
			console.log('got reply! ', dataFromServer);
			if (dataFromServer.type === 'message') {
				setVol(dataFromServer.voltage);
				setCur(dataFromServer.current);
				setTemp(dataFromServer.temperature);
				setLight(dataFromServer.light);
				setRtc(dataFromServer.rtc);
			}
		};
		return () => websocket.current?.close();
	}, []);

	return (
		<div className="relative mx-auto w-[350px] pt-10">
			<img src={xcamp} alt="xcamp" className="absolute right-3 top-5 w-32" />
			{/* WELCOME */}
			<GlowingText
				blur="blur-[2px]"
				// eslint-disable-next-line react/jsx-closing-bracket-location
				className="gradient-purple mb-6 bg-clip-text text-4xl font-extrabold text-transparent">
				WELCOME!
			</GlowingText>
			<GlowingText blur="blur-[2px]" className="mb-8 text-4xl font-extrabold text-white">
				SMART PJU #12
			</GlowingText>
			{/* GRID */}
			<div className="grid grid-cols-2 gap-x-3 gap-y-5">
				<DateTime unix={rtc} />

				<Card type="voltage" value={vol} icon={volIcon} />
				<Card type="current" value={cur} icon={curIcon} />
				<Card type="temperature" value={temp} icon={tempIcon} />
				<Card type="light level" value={light} icon={lightIcon} />
			</div>
		</div>
	);
};

export default Home;

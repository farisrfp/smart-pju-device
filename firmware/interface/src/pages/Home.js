import React, { useState, useEffect } from 'react';
// import { w3cwebsocket as W3CWebSocket } from 'websocket';
import axios from 'axios';

import Card from '../components/Card';
import DateTime from '../components/DateTime';
import GlowingText from '../components/utils/GlowingText';

import xcamp from '../asset/Group 20.png';

const Home = () => {
	// const websocket = useRef(null);
	const [vol, setVol] = useState(220);
	const [cur, setCur] = useState(1.22);
	const [temp, setTemp] = useState(25);
	const [light, setLight] = useState(40);
	const [rtc, setRtc] = useState(1685435353);

	const CardItem = [
		{
			type: 'voltage',
			value: vol,
		},
		{
			type: 'current',
			value: cur,
		},
		{
			type: 'temperature',
			value: temp,
		},
		{
			type: 'light level',
			value: light,
		},
	];

	useEffect(() => {
		// get data from api
		axios
			.get('/sensor')
			.then(res => {
				const { voltage, current, temperature, light } = res.data;
				setVol(voltage);
				setCur(current);
				setTemp(temperature);
				setLight(light);
			})
			.catch(err => {
				console.log(err);
			});
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

				{CardItem.map(item => (
					<Card key={item.type} type={item.type} value={item.value} />
				))}
			</div>
		</div>
	);
};

export default Home;

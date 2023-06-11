import React from 'react';
import { Link } from 'react-router-dom';

// ctrl alt a

import Menu from '../components/Menu';

const SettingMenu = ['sensor', 'lorawan', 'about'];

const Setting = () => {
	// const [voltage, setVoltage] = useState(256);
	// const [current, setCurrent] = useState(59);
	// const [appEUI, setAppEUI] = useState('0000000000000000');
	// const [devEUI, setDevEUI] = useState('C3E005D07ED5B370');
	// const [appKey, setAppKey] = useState('44F393F26FA81504493D6699523E2251');

	return (
		<>
			<div className="py-5" />
			<div className="mx-auto h-[75%] w-[350px] rounded-2xl bg-primary p-5 text-center">
				<h1 className="mb-8 pt-5 text-2xl font-bold tracking-widest text-white">SETTING</h1>

				<div className="flex flex-col ">
					{SettingMenu.map((item, index) => (
						<Link to={`/setting/${item}`} key={index} className="mb-5">
							<Menu type={item} />
						</Link>
					))}
				</div>
			</div>
		</>
	);
};

export default Setting;

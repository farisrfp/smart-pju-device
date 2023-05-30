import React from 'react';

import Card from '../components/Card';

import volIcon from '../asset/image 7.png';

const Setting: React.FC = () => {
	return (
		<>
			<div className="mx-auto flex h-80 w-[350px] flex-wrap items-center justify-between">
				<Card type="temperature" value={60} icon={volIcon} />
				<Card type="temperature" value={60} icon={volIcon} />
				<Card type="temperature" value={60} icon={volIcon} />
				<Card type="temperature" value={60} icon={volIcon} />
			</div>
		</>
	);
};

export default Setting;

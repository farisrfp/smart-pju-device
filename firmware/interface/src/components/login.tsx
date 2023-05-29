import React from 'react';

import xcamp from '../asset/xcamp.png';

const Login: React.FC = () => {
	return (
		<div className="mx-auto -mt-20 w-80 rounded-xl bg-primary">
			<div className="my-10 flex-col">
				<img src={xcamp} alt="XCamp" className="mx-auto w-40" />
				<div className="mx-auto mt-10 w-48 flex-col">
					{/* Form Input */}
					<input
						type="text"
						placeholder="Username"
						className="mb-5 h-10 w-full rounded-lg border-2 border-gray-500 bg-primary px-2"
					/>
					<input
						type="password"
						placeholder="Password"
						className="mb-7 h-10 w-full rounded-lg border-2 border-gray-500 bg-primary px-2"
					/>
					{/* Form Button */}
					<input
						type="submit"
						value="LOGIN"
						className="bg-secondary float-right mb-10 rounded-full bg-slate-500 px-6 py-2 tracking-widest text-white"
					/>
				</div>
			</div>
		</div>
	);
};

export default Login;

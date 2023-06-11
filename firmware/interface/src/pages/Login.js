import React, { useState } from 'react';

import xcamp from '../assets/xcamp.png';
import Button from '../components/Button';

const Login = () => {
	return (
		<div className="absolute left-1/2 top-1/2 w-[350px] -translate-x-1/2 -translate-y-1/2 transform rounded-lg bg-primary/70">
			<div className="flex-col">
				<div className="relative mx-auto mt-10 h-fit w-fit">
					<img src={xcamp} alt="XCamp" className="absolute -z-10 w-52 blur-[2px]" />
					<img src={xcamp} alt="XCamp" className="w-52" />
				</div>
				<div className="mx-auto mt-10 w-60 flex-col">
					{/* Form Input */}
					<form>
						<input
							type="text"
							placeholder="Username"
							name="username"
							className="mb-3 h-10 w-full rounded-lg border border-white bg-primary px-2 text-sm tracking-wide text-white"
						/>
						<input
							type="password"
							placeholder="Password"
							name="password"
							className="mb-8 h-10 w-full rounded-lg border border-white bg-primary px-2 text-sm tracking-wide text-white"
						/>
						{/* Form Button */}
						<Button className="float-right mb-10 bg-slate-400 px-6 tracking-widest">
							<p className="gradient-techdays bg-clip-text font-extrabold text-transparent">
								LOGIN
							</p>
						</Button>
					</form>
				</div>
			</div>
		</div>
	);
};

export default Login;

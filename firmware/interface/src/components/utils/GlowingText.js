import React from 'react';

const GlowingText = ({ children, className, blur }) => {
	return (
		<div className="relative h-fit w-fit">
			<p className={`absolute left-0 ${blur} ${className}`}>{children}</p>
			<p className={`relative z-10 ${className}`}>{children}</p>
		</div>
	);
};

export default GlowingText;

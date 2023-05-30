import React from 'react';

interface GlowingTextProps {
	className: string;
	blur: string;
	children?: React.ReactNode;
}

const GlowingText: React.FC<GlowingTextProps> = ({ className, blur, children }) => {
	return (
		<div className="relative w-fit">
			<p className={`absolute left-0 ${blur} ${className}`}>{children}</p>
			<p className={`relative z-10 ${className}`}>{children}</p>
		</div>
	);
};

export default GlowingText;

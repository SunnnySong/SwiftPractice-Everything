//
//  ViewController.swift
//  FSCalendar_practice
//
//  Created by 송선진 on 2022/10/17.
//

import UIKit
import FSCalendar
import SnapKit

class ViewController: UIViewController {

    let calendar = CalendarView()
    
    let dataView: UIView = {
        let view = UIView()
        view.backgroundColor = UIColor(hex: "fcc0ec", alpha: 0.72)
        view.layer.cornerRadius = 20
        view.clipsToBounds = true
        return view
    }()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        layoutCal()
    }

    private func layoutCal() {
        view.addSubview(calendar)
        calendar.snp.makeConstraints { make in
            make.left.right.equalToSuperview().inset(15)
            make.top.equalToSuperview().inset(100)
            make.height.equalTo(330)
        }
        
        view.addSubview(dataView)
        dataView.snp.makeConstraints { make in
            make.top.equalTo(calendar.snp.bottom).offset(20)
            make.left.right.equalToSuperview().inset(15)
            make.height.equalTo(100)
        }
    }
}


// HEX color
extension UIColor {
    convenience init(hex: String, alpha: CGFloat = 1.0) {
        var hexFormatted: String = hex.trimmingCharacters(in: CharacterSet.whitespacesAndNewlines).uppercased()
        if hexFormatted.hasPrefix("#") {
            hexFormatted = String(hexFormatted.dropFirst())
        }
        assert(hexFormatted.count == 6, "Invalid hex code used.")
        var rgbValue: UInt64 = 0
        Scanner(string: hexFormatted).scanHexInt64(&rgbValue)
        self.init(red: CGFloat((rgbValue & 0xFF0000) >> 16) / 255.0, green: CGFloat((rgbValue & 0x00FF00) >> 8) / 255.0, blue: CGFloat(rgbValue & 0x0000FF) / 255.0, alpha: alpha)
    }
}

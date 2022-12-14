//
//  Calendar.swift
//  FSCalendar_practice
//
//  Created by 송선진 on 2022/10/18.
//

import UIKit
import FSCalendar
import SnapKit


enum extensionNum {
    case up
    case down
}

class CalendarView: FSCalendar {

    // calendar week / month 변경하는 버튼 관련 변수
    var extensionState = extensionNum.down
    
    // MARK: - Properties
    
    private lazy var extensionButton: UIButton = {
        var button = UIButton()
        button.setImage(UIImage(systemName: "chevron.up"), for: .normal)
        button.tintColor = .black
        button.addTarget(self, action: #selector(handleExtensionBtn), for: .touchUpInside)
        return button
    }()
    
    private lazy var preMonthButton: UIButton = {
        var button = UIButton()
        button.setImage(UIImage(systemName: "chevron.left"), for: .normal)
        button.tintColor = .systemBlue
        button.addTarget(self, action: #selector(handlePreMonthBtn), for: .touchUpInside)
        return button
    }()
    
    private lazy var nextMonthButton: UIButton = {
        var button = UIButton()
        button.setImage(UIImage(systemName: "chevron.right"), for: .normal)
        button.tintColor = .systemBlue
        button.addTarget(self, action: #selector(handleNextMonthBtn), for: .touchUpInside)
        return button
    }()

    
    // MARK: - Lifecycle
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        extensionState = .down
        
        configuareCal()
        configuareExtensionBtn()
        configuareMonthBtn()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
        extensionSpacing()
    }
    
    // MARK: - Helpers
    
    func extensionSpacing() {
        // 확장 화살표 넣기 위한 bottom 공간 확보
        if scope == .month {
            self.snp.updateConstraints { make in
                make.height.equalTo(360)
            }
        } else {
            self.snp.updateConstraints { make in
                make.height.equalTo(160)
            }
        }
    }
    
    func configuareCal() {
        
        backgroundColor = UIColor(hex: "5294F9", alpha: 0.12)
        layer.cornerRadius = 20
        clipsToBounds = true
        
        // header
        appearance.headerDateFormat = "YYYY년 MM월"
        // header 에 흐릿하게 보이는 년, 월 없애기
        appearance.headerMinimumDissolvedAlpha = 0
        // header 왼쪽 정렬
        appearance.headerTitleAlignment = .left
        appearance.headerTitleOffset = CGPoint(x: -70, y: 0)
        // NSAttributedString을 사용하여 년과 월의 색을 각각 변경하고 싶었지만 실패, 나중 시도!!!!!!!!
//        appearance.headerTitleColor = .black
        appearance.calendar.headerHeight = 45
        
    }
    
    func configuareMonthBtn() {
        addSubview(preMonthButton)
        addSubview(nextMonthButton)
        
        // autoLayout 잡을때, calendarHeaderView가 아니라 date에 걸어야함. 나중 시도!!!!!!!!
        preMonthButton.snp.makeConstraints { make in
            make.left.equalTo(calendarHeaderView).inset(10)
            make.centerY.equalTo(calendarHeaderView)
        }
        
        nextMonthButton.snp.makeConstraints { make in
            make.left.equalTo(calendarHeaderView).offset(120)
            make.centerY.equalTo(calendarHeaderView)
        }
    }
    
    func configuareExtensionBtn() {
        addSubview(extensionButton)
        
        // autoLayout
        extensionButton.snp.makeConstraints { make in
            make.bottom.equalToSuperview().inset(10)
            make.centerX.equalToSuperview()
        }
    }
    
    @objc func handleExtensionBtn() {
        if extensionState == .down {
            extensionState = .up
            self.setScope(.week, animated: true)
            extensionButton.setImage(UIImage(systemName: "chevron.down"), for: .normal)
        } else {
            extensionState = .down
            self.setScope(.month, animated: true)
            extensionButton.setImage(UIImage(systemName: "chevron.up"), for: .normal)
        }
    }
    
    // 스크롤로 월 변경 대신 <> 로 변경. 나중 시도!!!!!!!!
    @objc func handlePreMonthBtn() {
        print("pre")
    }
    
    @objc func handleNextMonthBtn() {
        print("next")
    }
}
